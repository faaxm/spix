/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Scene/Item.h>

#include <map>

namespace spix {

class MockItem : public Item {
public:
    MockItem(Size size);

    // Item interface
    Size size() const override;
    std::string stringProperty(const std::string& name) const override;
    void setStringProperty(const std::string& name, const std::string& value) override;
    bool visible() const override;

    // MockItem specials
    std::map<std::string, std::string>& stringProperties();

private:
    Size m_size;
    std::map<std::string, std::string> m_stringProperties;
};

} // namespace spix
