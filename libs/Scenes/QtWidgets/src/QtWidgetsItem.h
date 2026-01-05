/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/Scene/Item.h>

class QObject;
class QWidget;

namespace spix {

class QtWidgetsItem : public Item {
public:
    QtWidgetsItem() = delete;
    explicit QtWidgetsItem(QWidget* widget);

    Size size() const override;
    Point position() const override;
    Rect bounds() const override;
    std::string stringProperty(const std::string& name) const override;
    void setStringProperty(const std::string& name, const std::string& value) override;
    bool invokeMethod(const std::string& method, const std::vector<Variant>& args, Variant& ret) override;
    bool visible() const override;

    QWidget* qwidget();
    const QWidget* qwidget() const;

private:
    QObject* qobject();
    const QObject* qobject() const;

    QWidget* m_widget;
};

} // namespace spix
