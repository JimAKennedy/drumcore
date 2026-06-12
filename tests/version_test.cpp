//------------------------------------------------------------------------
// Copyright(c) 2025-2026 JK Digital.
// SPDX-License-Identifier: Apache-2.0
//------------------------------------------------------------------------

#include <drumcore/drumcore.h>
#include <gtest/gtest.h>

#include <cstring>

TEST(Version, MacrosAreDefined) {
    EXPECT_EQ(DRUMCORE_VERSION_MAJOR, 0);
    EXPECT_EQ(DRUMCORE_VERSION_MINOR, 1);
    EXPECT_EQ(DRUMCORE_VERSION_PATCH, 0);
    EXPECT_STREQ(DRUMCORE_VERSION_STRING, "0.1.0");
}

TEST(Version, StringIsNonEmpty) {
    EXPECT_GT(std::strlen(DRUMCORE_VERSION_STRING), 0u);
}
