/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Scene/Item.h>

class QQuickItem;

namespace spix {

class QtItem : public Item {
public:
    QtItem() = delete;
    QtItem(QQuickItem* item);

    Size size() const override;
    std::string stringProperty(const std::string& name) const override;
    bool visible() const override;

    QQuickItem* qquickitem();

private:
    QQuickItem* m_item;
};

} // namespace spix
