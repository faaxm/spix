/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <gtest/gtest.h>

#include <Spix/Data/PasteboardContent.h>

TEST(PasteboardContentTest, AddUrls)
{
    uibot::PasteboardContent content;

    content.addUrl("file://some/file");
    content.addUrl("file://other/file");

    EXPECT_EQ(content.urls().size(), 2);
}

TEST(PasteboardContentTest, HelperMakeWithUrls)
{
    auto content = uibot::makePasteboardContentWithUrls({"file://some/file", "file://other/file/here"});

    EXPECT_EQ(content.urls().size(), 2);
    EXPECT_EQ(content.urls().at(0), "file://some/file");
    EXPECT_EQ(content.urls().at(1), "file://other/file/here");
}
