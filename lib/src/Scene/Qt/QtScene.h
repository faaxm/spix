/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <QByteArray>
#include <QQuickItem>
#include <Scene/Qt/QtEvents.h>
#include <Scene/Scene.h>

#include <map>
#include <string>

class QQuickWindow;

namespace spix {

class QtEventFilter;
class ItemPath;

using ObjectIds = QVector<QObject*>;

class QtScene : public Scene {
public:
    QtScene();
    ~QtScene();
    // Request objects
    std::unique_ptr<Item> itemAtPath(const ItemPath& path) override;
    ItemPath shortPath(ItemPath oldPath, QQuickItem* itemPath);

    // Events
    Events& events() override;

    // Tasks
    void takeScreenshot(const ItemPath& targetItem, const std::string& filePath) override;
    std::string takeScreenshotAsBase64(const ItemPath& targetItem);
    ObjectIds recursiveItemsAt(QQuickItem* parent, const QPointF& pos, int& bestCandidate, bool parentIsGoodCandidate);
    QRectF combinedChildrenRect(QQuickItem* object) const;

private:
    QtEvents m_events;
    bool m_eventFilterInstalled = false;
    QtEventFilter* m_filter;
    bool itemHasContents(QQuickItem* item);
    bool isGoodCandidateItem(QQuickItem* item, bool ignoreItemHasContents);
};

} // namespace spix
