/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "QtScene.h"
#include "FindQtItem.h"
#include "QtEventFilter.h"
#include "Scene/Item.h"

#include <Scene/Qt/QtItem.h>
#include <Scene/Qt/QtItemTools.h>
#include <Spix/Data/ItemPath.h>

#include <QBuffer>
#include <QByteArray>
#include <QGuiApplication>
#include <QObject>
#include <QQuickItem>
#include <QQuickWindow>
#include <algorithm>
#include <qglobal.h>


namespace {

QString GetObjectFinalName(QObject* obj, bool top){
    auto token = spix::qt::GetObjectName(obj);

    if(obj->inherits("QQuickText")){
        auto var = obj->property("text");
        if(!var.isValid()){
            return token;
        }
        // adds quotes around the text
        auto text = "\"" + var.toString() + "\"";
        if(top) return text;
    }

    // for certain types we want to return the text
    // not adding in now, but can add in later
    // if(token.contains("Button")) return text;
    // else if(token.contains("Label")) return text;

    return token;
}

spix::ItemPath ItemPathForObject(QObject* obj){
    auto currentItem = obj;
    QString path = "";

    while(currentItem != nullptr) {
        auto token = spix::qt::GetObjectName(currentItem);
        int sameNameNumber = 0;

        const auto currentQuickItem = qobject_cast<QQuickItem*>(currentItem);

        if(currentItem->parent() != nullptr){
            auto siblings = currentItem->parent()->children();
            for(const auto child : siblings) {
                sameNameNumber += (token == spix::qt::GetObjectName(child));
            }
        }else if(currentQuickItem != nullptr && currentQuickItem->parentItem() != nullptr) {
            auto siblings = currentQuickItem->parentItem()->children();
            for(const auto child : siblings) {
                sameNameNumber += (token == spix::qt::GetObjectName(child));
            }
        }
        if((currentItem->parent() == nullptr)
           && (currentQuickItem != nullptr && currentQuickItem->parentItem() != nullptr)) {
            currentItem = currentQuickItem->parentItem();
        } else {
            currentItem = currentItem->parent();
        }

        if (sameNameNumber > 1) {
            continue;
        }

        path = token + "/" + path;
    }

    return spix::ItemPath(path.toStdString());
}

} // namespace


namespace spix {


QtScene::QtScene() {
    m_filter = new QtEventFilter(qGuiApp);

    QObject::connect(qGuiApp, &QGuiApplication::focusWindowChanged, qGuiApp, [this](QWindow* window) {
        if(!m_eventFilterInstalled) {
            m_eventFilterInstalled = true;
            window->installEventFilter(m_filter);

            QObject::connect(m_filter, &QtEventFilter::pickerModeEntered, m_filter, []() {
                     QGuiApplication::setOverrideCursor(QCursor(Qt::CrossCursor));
            });
            QObject::connect(m_filter, &QtEventFilter::pickerModeExited, m_filter, []() {
                     QGuiApplication::restoreOverrideCursor();
            });

            auto quickWindow = qobject_cast<QQuickWindow*>(window);
            QObject::connect(m_filter, &QtEventFilter::pickClick, m_filter, [this, quickWindow](QMouseEvent* e){
                int bestCandidate = -1;
                bool parentIsGoodCandidate = true;
                auto objects = recursiveItemsAt(quickWindow->contentItem(), e->pos(), bestCandidate, parentIsGoodCandidate);

                if(objects.size() == 1) {
                    auto quickItem = qobject_cast<QQuickItem*>(objects[0]);
                    // quickItem->setOpacity(0.5);
                    auto itemPath = ItemPathForObject(quickItem);

                    auto newPath = shortPath(itemPath, quickItem);
                    // prints the path and surrounds it with quotes
                    qDebug().setAutoInsertSpaces(false);
                    auto pathString = "\'" + newPath.string() + "\'";
                    qDebug() << "Path: " << pathString.c_str();
                }
            });
        }
    });
}

QtScene::~QtScene(){
    delete m_filter;
}

spix::ItemPath QtScene::shortPath(ItemPath oldPath, QQuickItem* oldItem){
    spix::ItemPath newPath;

    auto components = oldPath.components();

    // add the first
    if(components.size() > 1){
        newPath = spix::ItemPath(components.at(0).string());
    } else {
        return oldPath;
    }

    for(auto p = components.begin() + 1; p != components.end() - 1; p++){
        // iterate through the middle of the components
        auto next = p + 1;

        // item
        auto path = spix::ItemPath(newPath.string() + "/" + p->string() + "/" + next->string());
        auto shortPath = spix::ItemPath(newPath.string() + "/" + next->string());

        auto item = qt::GetQQuickItemAtPath(path);
        auto shortItem = qt::GetQQuickItemAtPath(shortPath);

        
        if(shortItem == nullptr || item != shortItem){
            // need to add this item to path
            newPath = spix::ItemPath(newPath.string() + "/" + p->string());
        }
        // otherwise continue without this item
    }

    // add the last item
    auto name = GetObjectFinalName(oldItem, true);
    newPath = spix::ItemPath(newPath.string() + "/" + name.toStdString());

    return newPath;
}

std::unique_ptr<Item> QtScene::itemAtPath(const ItemPath& path)
{
    // Find item within window
    auto item = qt::GetQQuickItemAtPath(path);

    if (!item) {
        return {};
    }

    return std::make_unique<QtItem>(item);
}

Events& QtScene::events()
{
    return m_events;
}

void QtScene::takeScreenshot(const ItemPath& targetItem, const std::string& filePath)
{
    auto item = qt::GetQQuickItemAtPath(targetItem);
    if (!item) {
        return;
    }

    // take screenshot of the full window
    auto windowImage = item->window()->grabWindow();

    // get the rect of the item in window space in pixels, account for the device pixel ratio
    QRectF imageCropRectItemSpace {0, 0, item->width(), item->height()};
    auto imageCropRectF = item->mapRectToScene(imageCropRectItemSpace);
    QRect imageCropRect(imageCropRectF.x() * windowImage.devicePixelRatio(),
        imageCropRectF.y() * windowImage.devicePixelRatio(), imageCropRectF.width() * windowImage.devicePixelRatio(),
        imageCropRectF.height() * windowImage.devicePixelRatio());

    // crop the window image to the item rect
    auto image = windowImage.copy(imageCropRect);
    image.save(QString::fromStdString(filePath));
}

std::string QtScene::takeScreenshotAsBase64(const ItemPath& targetItem)
{
    auto item = qt::GetQQuickItemAtPath(targetItem);
    if (!item) {
        return "";
    }

    // take screenshot of the full window
    auto windowImage = item->window()->grabWindow();

    // get the rect of the item in window space in pixels, account for the device pixel ratio
    QRectF imageCropRectItemSpace {0, 0, item->width(), item->height()};
    auto imageCropRectF = item->mapRectToScene(imageCropRectItemSpace);
    QRect imageCropRect(imageCropRectF.x() * windowImage.devicePixelRatio(),
        imageCropRectF.y() * windowImage.devicePixelRatio(), imageCropRectF.width() * windowImage.devicePixelRatio(),
        imageCropRectF.height() * windowImage.devicePixelRatio());

    // crop the window image to the item rect
    auto image = windowImage.copy(imageCropRect);
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    buffer.close();

    return byteArray.toBase64().toStdString();
}

bool QtScene::itemHasContents(QQuickItem* item) {
    return item->flags().testFlag(QQuickItem::ItemHasContents);
}
bool QtScene::isGoodCandidateItem(QQuickItem* item, bool ignoreItemHasContents = false) {
    return !(
           !item->isVisible()
        || qFuzzyCompare(item->opacity() + qreal(1.0), qreal(1.0))
        || (!ignoreItemHasContents && !itemHasContents(item)));
}


QRectF QtScene::combinedChildrenRect(QQuickItem* obj) const {
    auto rect = obj->childrenRect();

    for(const auto child : obj->childItems()) {
        auto childRect = child->childrenRect();
        QPointF childGlobalPos = child->mapToScene(QPointF(0, 0));
        QPointF localChildPos = obj->mapFromScene(childGlobalPos);
        childRect.moveTopLeft((localChildPos.toPoint()));
        rect = rect.united(childRect);
    }

    return rect;
}

ObjectIds QtScene::recursiveItemsAt(QQuickItem* parent, const QPointF& pos, int& bestCandidate, bool parentIsGoodCandidate){
    Q_ASSERT(parent);
    ObjectIds objects;

    bestCandidate = -1;
    if(parentIsGoodCandidate) {
        parentIsGoodCandidate = isGoodCandidateItem(parent, true);
    }

    auto childItems = parent->childItems();
    std::stable_sort(childItems.begin(), childItems.end(), [](QQuickItem* a, QQuickItem* b){
        return a->z() < b->z();
    });


    for(int i = childItems.size() - 1; i >= 0; i--) {
        const auto child = childItems.at(i);
        const auto requestedPoint = parent->mapToItem(child, pos);

        if(!child->childItems().isEmpty() && (child->contains(requestedPoint) || combinedChildrenRect(child).contains(requestedPoint))){
            const int count = objects.count();
            int bc = 0;
            objects << recursiveItemsAt(child, requestedPoint, bc, parentIsGoodCandidate);
            if(bestCandidate == -1 && parentIsGoodCandidate && bc != -1){
                bestCandidate = count + bc;
            }
        }

        if(child->contains(requestedPoint)) {
            if(bestCandidate == -1 && parentIsGoodCandidate && isGoodCandidateItem(child)){
                bestCandidate = objects.count();
            }
            objects << child;
        }
        if(bestCandidate != -1) break;
    }

    if(bestCandidate == -1 && parentIsGoodCandidate && itemHasContents(parent)){
        bestCandidate = objects.count();
    }

    objects << parent;

    if(bestCandidate != -1) {
        objects = ObjectIds() << objects[bestCandidate];
        bestCandidate = 0;
    }

    return objects;
}


} // namespace spix
