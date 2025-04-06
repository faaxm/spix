/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/Scene/Item.h>
#include <variant>

class QObject;
class QQuickItem;
class QQuickWindow;

namespace spix {

class QtItem : public Item {
public:
    QtItem() = delete;
    QtItem(QQuickItem* item);
    QtItem(QQuickWindow* window);

    Size size() const override;
    Point position() const override;
    Rect bounds() const override;
    std::string stringProperty(const std::string& name) const override;
    void setStringProperty(const std::string& name, const std::string& value) override;
    bool invokeMethod(const std::string& method, const std::vector<Variant>& args, Variant& ret) override;
    bool visible() const override;

    QQuickItem* qquickitem();
    const QQuickItem* qquickitem() const;

private:
    QObject* qobject();
    const QObject* qobject() const;

    std::variant<QQuickItem*, QQuickWindow*> m_item;
};

} // namespace spix
