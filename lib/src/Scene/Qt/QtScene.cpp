/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "QtScene.h"
#include "QtEventFilter.cpp"

#include <Scene/Qt/QtItem.h>
#include <Scene/Qt/QtItemTools.h>
#include <Spix/Data/ItemPath.h>

#include <QBuffer>
#include <QByteArray>
#include <QGuiApplication>
#include <QObject>
#include <QQuickItem>
#include <QQuickWindow>

namespace {

QString getNameForObject(QObject* object)
{
    QString name;
    if (spix::qt::PropertValueByObject(object, QString::fromStdString("text")) != "") {
        name = "\"" + spix::qt::PropertValueByObject(object, QString::fromStdString("text")) + "\"";
    } else if (spix::qt::PropertValueByObject(object, QString::fromStdString("source")) != "") {
        name = "(source=" + spix::qt::PropertValueByObject(object, QString::fromStdString("source")) + ")";
    } else if (spix::qt::GetObjectName(object) != "") {
        name = spix::qt::GetObjectName(object);
    } else {
        name = "#" + spix::qt::TypeByObject(object);
    }

    return name;
}

spix::ItemPath ItemPathForObject(QObject* object)
{
    auto currentItem = object;
    QString path = "";

    while (currentItem != nullptr) {
        // take object name if given
        auto token = getNameForObject(currentItem);
        int sameNameNumber = 0;

        const auto currentQuickItem = qobject_cast<QQuickItem*>(currentItem);

        if (currentItem->parent() != nullptr) {
            auto siblings = currentItem->parent()->children();
            for (const auto child : siblings) {
                if (token == getNameForObject(child)) {
                    sameNameNumber++;
                }
            }
        } else if (currentQuickItem != nullptr && currentQuickItem->parentItem()) {
            auto siblings = currentQuickItem->parentItem()->children();
            for (const auto child : siblings) {
                if (token == getNameForObject(child)) {
                    sameNameNumber++;
                }
            }
        }

        if ((currentItem->parent() == nullptr)
            && (currentQuickItem != nullptr && currentQuickItem->parentItem() != nullptr)) {
            currentItem = currentQuickItem->parentItem();
        } else {
            currentItem = currentItem->parent();
        }

        if (sameNameNumber > 1) {
            continue;
        }
        // add id to front
        path = token + "/" + path;
    }

    return spix::ItemPath(path.toStdString());
}

QQuickWindow* getQQuickWindowWithName(const std::string& name)
{
    QString qtName = QString::fromStdString(name);
    QQuickWindow* foundWindow = nullptr;

    auto windows = QGuiApplication::topLevelWindows();
    for (const auto& window : windows) {
        QQuickWindow* qquickWindow = qobject_cast<QQuickWindow*>(window);
        if (qquickWindow && (spix::qt::GetObjectName(qquickWindow) == qtName)) {
            foundWindow = qquickWindow;
            break;
        }
    }

    return foundWindow;
}

QQuickItem* getQQuickItemWithRoot(const spix::ItemPath& path, QObject* root)
{
    if (path.length() == 0) {
        return nullptr;
    }
    if (!root) {
        return nullptr;
    }

    auto rootClassName = root->metaObject()->className();
    auto itemName = path.rootComponent();
    QQuickItem* subItem = nullptr;
    QVector<QObject*> subItems = {};

    if (itemName.compare(0, 1, ".") == 0) {
        auto propertyName = itemName.substr(1);
        QVariant propertyValue = root->property(propertyName.c_str());
        if (propertyValue.isValid()) {
            subItem = propertyValue.value<QQuickItem*>();
        }

    } else if (itemName.compare(0, 1, "\"") == 0) {
        // remove ""
        size_t found = itemName.find("\"");
        auto searchText = itemName.substr(found + 1, itemName.length() - 2);

        subItem = spix::qt::FindChildItem<QQuickItem*>(
            root, itemName.c_str(), QString::fromStdString("text"), QString::fromStdString(searchText), {});

    } else if (itemName.compare(0, 1, "#") == 0) {
        // remove #
        size_t found = itemName.find("#");
        auto type = QString::fromStdString(itemName.substr(found + 1));

        subItems = spix::qt::FindChildItems(root, type);

        for (const auto item : subItems) {
            auto foundItem = getQQuickItemWithRoot(path.subPath(1), item);
            if (foundItem != nullptr) {
                return foundItem;
            }
        }
    } else if (itemName.compare(0, 1, "(") == 0) {
        // remove ()
        size_t foundBracketSign = itemName.find('(');
        auto searchText = itemName.substr(foundBracketSign + 1, itemName.length() - 2);

        // Split in to property and value
        size_t foundEqualSign = searchText.find('=');
        auto property = QString::fromStdString(searchText.substr(0, foundEqualSign));
        auto value = QString::fromStdString(searchText.substr(foundEqualSign + 1));

        subItem = spix::qt::FindChildItem<QQuickItem*>(root, itemName.c_str(), property, value, {});

    } else if (rootClassName == spix::qt::repeater_class_name) {
        QQuickItem* repeater = static_cast<QQuickItem*>(root);
        subItem = spix::qt::RepeaterChildWithName(repeater, QString::fromStdString(itemName));
    } else {
        subItem = spix::qt::FindChildItem<QQuickItem*>(root, itemName.c_str());
    }

    if (path.length() == 1) {
        return subItem;
    }

    return getQQuickItemWithRoot(path.subPath(1), subItem);
}

QQuickItem* getQQuickItemAtPath(const spix::ItemPath& path)
{
    auto windowName = path.rootComponent();
    QQuickWindow* itemWindow = getQQuickWindowWithName(windowName);
    QQuickItem* item = nullptr;

    if (!itemWindow) {
        return nullptr;
    }

    if (path.length() > 1) {
        item = getQQuickItemWithRoot(path.subPath(1), itemWindow);
    } else {
        item = itemWindow->contentItem();
    }

    return item;
}

} // namespace

namespace spix {

/**
Create a QtScene with an EventFilter
**/
QtScene::QtScene()
{
    m_filter = new QtEventFilter(qGuiApp);

    QObject::connect(qGuiApp, &QGuiApplication::focusWindowChanged, qGuiApp, [this](QWindow* window) {
        if (m_eventFilterInstalled == false) {
            m_eventFilterInstalled = true;
            window->installEventFilter(m_filter);

            QObject::connect(m_filter, &QtEventFilter::pickerModeEntered, m_filter,
                []() { QGuiApplication::setOverrideCursor(QCursor(Qt::CrossCursor)); });

            QObject::connect(m_filter, &QtEventFilter::pickerModeExited, m_filter,
                []() { QGuiApplication::restoreOverrideCursor(); });

            auto quickWindow = qobject_cast<QQuickWindow*>(window);
            QObject::connect(m_filter, &QtEventFilter::pickClick, m_filter, [this, quickWindow](QMouseEvent* event) {
                int bestCanidate = -1;
                bool parentIsGoodCandidate = true;
                auto objects
                    = recursiveItemsAt(quickWindow->contentItem(), event->pos(), bestCanidate, parentIsGoodCandidate);

                if (objects.size() == 1) {
                    auto quickItem = qobject_cast<QQuickItem*>(objects[0]);
                    quickItem->setOpacity(0.5);
                    auto itemPath = ItemPathForObject(quickItem);

                    auto newPath = shortPath(itemPath, quickItem);
                    qDebug() << "Path: " << QString::fromUtf8(newPath.string().c_str());
                }
            });
        }
    });
}

QtScene::~QtScene()
{
    delete m_filter;
}

std::unique_ptr<Item> QtScene::itemAtPath(const ItemPath& path)
{
    auto windowName = path.rootComponent();
    QQuickWindow* itemWindow = getQQuickWindowWithName(windowName);

    if (!itemWindow || !itemWindow->contentItem()) {
        return {};
    }
    if (path.length() <= 1) {
        return std::make_unique<QtItem>(itemWindow);
    }

    auto item = getQQuickItemWithRoot(path.subPath(1), itemWindow);

    if (!item) {
        return {};
    }
    return std::make_unique<QtItem>(item);
}

spix::ItemPath QtScene::shortPath(ItemPath oldPath, QQuickItem* oldItem)
{
    spix::ItemPath newItemPath;
    QQuickItem* newItem;
    std::string tempNewItemPath = oldPath.rootComponent();

    auto partsPath = oldPath.components();

    for (auto part = partsPath.rbegin(); part != std::prev(partsPath.rend()); ++part) {

        size_t found = tempNewItemPath.find("/");
        if (found != std::string::npos) {
            auto subfix = tempNewItemPath.substr(found + 1, tempNewItemPath.length());
            tempNewItemPath = oldPath.rootComponent().append("/").append(part->c_str()).append("/").append(subfix);
        } else {
            tempNewItemPath.append("/").append(part->c_str());
        }

        auto newItem = getQQuickItemAtPath(ItemPath(tempNewItemPath));

        if (newItem == oldItem) {
            newItemPath = ItemPath(tempNewItemPath);
            return newItemPath;
        }
    }
    return oldPath;
}

Events& QtScene::events()
{
    return m_events;
}

void QtScene::takeScreenshot(const ItemPath& targetItem, const std::string& filePath)
{
    auto item = getQQuickItemAtPath(targetItem);
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
    auto item = getQQuickItemAtPath(targetItem);
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

bool QtScene::itemHasContents(QQuickItem* item)
{
    return item->flags().testFlag(QQuickItem::ItemHasContents);
}

bool QtScene::isGoodCandidateItem(QQuickItem* item, bool ignoreItemHasContents = false)
{
    return !(!item->isVisible() || qFuzzyCompare(item->opacity() + qreal(1.0), qreal(1.0))
        || (!ignoreItemHasContents && !itemHasContents(item)));
}

QRectF QtScene::combinedChildrenRect(QQuickItem* object) const
{
    auto rect = object->childrenRect();

    for (const auto child : object->childItems()) {
        // Get Rect from Childitems
        auto childRect = child->childrenRect();

        // Get Global positon of childRect
        QPointF childGlobalPos = child->mapToScene(QPointF(0, 0));

        // Convert global position to local coordinates of the parent object
        QPointF localChildPos = object->mapFromScene(childGlobalPos);

        // Adjust childRect to be in local coordinates of the parent object
        childRect.moveTopLeft(localChildPos.toPoint());

        // Adding the childRect to the rect
        rect = rect.united(childRect);
    }

    return rect;
}

/**
        Search for best matching Object on the Position.
**/
ObjectIds QtScene::recursiveItemsAt(
    QQuickItem* parent, const QPointF& pos, int& bestCandidate, bool parentIsGoodCandidate)
{
    Q_ASSERT(parent); // nulll check
    ObjectIds objects;

    bestCandidate = -1;
    if (parentIsGoodCandidate) {
        // inherit the parent item opacity when looking for a good candidate item
        // i.e. QQuickItem::isVisible is taking the parent into account already, but
        // the opacity doesn't - we have to do this manually
        // Yet we have to ignore ItemHasContents apparently, as the QQuickRootItem
        // at least seems to not have this flag set.
        parentIsGoodCandidate = isGoodCandidateItem(parent, true);
    }

    // sorting based on z positon
    auto childItems = parent->childItems();
    std::stable_sort(
        childItems.begin(), childItems.end(), [](QQuickItem* lhs, QQuickItem* rhs) { return lhs->z() < rhs->z(); });

    for (int i = childItems.size() - 1; i >= 0; --i) { // backwards to match z order
        const auto child = childItems.at(i);
        // position of child
        const auto requestedPoint = parent->mapToItem(child, pos);

        if (!child->childItems().isEmpty()
            && (child->contains(requestedPoint) || combinedChildrenRect(child).contains(requestedPoint))) {
            const int count = objects.count();
            int bc; // possibly better candidate among subChildren

            objects << recursiveItemsAt(child, requestedPoint, bc, parentIsGoodCandidate);

            if (bestCandidate == -1 && parentIsGoodCandidate && bc != -1) {
                bestCandidate = count + bc;
            }
        }

        if (child->contains(requestedPoint)) {
            if (bestCandidate == -1 && parentIsGoodCandidate && isGoodCandidateItem(child)) {
                bestCandidate = objects.count();
            }
            objects << child;
        }

        if (bestCandidate != -1) {
            break;
        }
    }

    if (bestCandidate == -1 && parentIsGoodCandidate && itemHasContents(parent)) {
        bestCandidate = objects.count();
    }

    objects << parent;

    if (bestCandidate != -1) {
        objects = ObjectIds() << objects[bestCandidate];
        bestCandidate = 0;
    }

    return objects;
}

} // namespace spix
