//------------------------------------------------------------------------
// GenreMapper Unit Tests
// Copyright(c) 2025-2026 JK Digital.
// SPDX-License-Identifier: Apache-2.0
//------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <drumcore/genremapper.h>

namespace JKDigital {
namespace test {

//------------------------------------------------------------------------
// GenreMapper::toGenreString Tests
//------------------------------------------------------------------------
TEST(GenreMapperTest, ToGenreString_ReturnsCorrectStrings) {
    EXPECT_STREQ(GenreMapper::toGenreString(DrumBar::Genre::Rock), "rock");
    EXPECT_STREQ(GenreMapper::toGenreString(DrumBar::Genre::Latin), "latin");
    EXPECT_STREQ(GenreMapper::toGenreString(DrumBar::Genre::Funk), "funk");
    EXPECT_STREQ(GenreMapper::toGenreString(DrumBar::Genre::Jazz), "jazz");
    EXPECT_STREQ(GenreMapper::toGenreString(DrumBar::Genre::HipHop), "hiphop");
    EXPECT_STREQ(GenreMapper::toGenreString(DrumBar::Genre::Afrobeat), "afrobeat");
    EXPECT_STREQ(GenreMapper::toGenreString(DrumBar::Genre::NewOrleans), "neworleans");
    EXPECT_STREQ(GenreMapper::toGenreString(DrumBar::Genre::Afrocuban), "afrocuban");
    EXPECT_STREQ(GenreMapper::toGenreString(DrumBar::Genre::Other), "other");
    EXPECT_STREQ(GenreMapper::toGenreString(DrumBar::Genre::Uncertain), "uncertain");
}

//------------------------------------------------------------------------
// GenreMapper::toDisplayString Tests
//------------------------------------------------------------------------
TEST(GenreMapperTest, ToDisplayString_ReturnsTitleCaseStrings) {
    EXPECT_STREQ(GenreMapper::toDisplayString(DrumBar::Genre::Rock), "Rock");
    EXPECT_STREQ(GenreMapper::toDisplayString(DrumBar::Genre::Latin), "Latin");
    EXPECT_STREQ(GenreMapper::toDisplayString(DrumBar::Genre::Funk), "Funk");
    EXPECT_STREQ(GenreMapper::toDisplayString(DrumBar::Genre::Jazz), "Jazz");
    EXPECT_STREQ(GenreMapper::toDisplayString(DrumBar::Genre::HipHop), "HipHop");
    EXPECT_STREQ(GenreMapper::toDisplayString(DrumBar::Genre::Afrobeat), "Afrobeat");
    EXPECT_STREQ(GenreMapper::toDisplayString(DrumBar::Genre::NewOrleans), "New Orleans");
    EXPECT_STREQ(GenreMapper::toDisplayString(DrumBar::Genre::Afrocuban), "Afrocuban");
    EXPECT_STREQ(GenreMapper::toDisplayString(DrumBar::Genre::Other), "Other");
    EXPECT_STREQ(GenreMapper::toDisplayString(DrumBar::Genre::Uncertain), "Uncertain");
}

//------------------------------------------------------------------------
// GenreMapper::fromNormalizedValue Tests
//------------------------------------------------------------------------
TEST(GenreMapperTest, FromNormalizedValue_MapsCorrectly) {
    // 10 genres: value 0.0-1.0 maps to enum 0-9
    EXPECT_EQ(GenreMapper::fromNormalizedValue(0.0), DrumBar::Genre::Rock);
    EXPECT_EQ(GenreMapper::fromNormalizedValue(0.09),
              DrumBar::Genre::Rock);  // 0.09 * 10 = 0.9 -> 0
    EXPECT_EQ(GenreMapper::fromNormalizedValue(0.11),
              DrumBar::Genre::Latin);  // 0.11 * 10 = 1.1 -> 1
    EXPECT_EQ(GenreMapper::fromNormalizedValue(0.45),
              DrumBar::Genre::HipHop);  // 0.45 * 10 = 4.5 -> 4
    EXPECT_EQ(GenreMapper::fromNormalizedValue(0.95),
              DrumBar::Genre::Uncertain);  // 0.95 * 10 = 9.5 -> 9
    EXPECT_EQ(GenreMapper::fromNormalizedValue(1.0), DrumBar::Genre::Uncertain);  // Clamped to max
}

//------------------------------------------------------------------------
// GenreMapper::toNormalizedValue Tests
//------------------------------------------------------------------------
TEST(GenreMapperTest, ToNormalizedValue_ReturnsCorrectValues) {
    // 10 genres (0-9), normalized to 0.0-1.0
    // Formula: index / (kNumGenres - 1) = index / 9
    EXPECT_DOUBLE_EQ(GenreMapper::toNormalizedValue(DrumBar::Genre::Rock), 0.0);              // 0/9
    EXPECT_DOUBLE_EQ(GenreMapper::toNormalizedValue(DrumBar::Genre::Latin), 1.0 / 9.0);       // 1/9
    EXPECT_DOUBLE_EQ(GenreMapper::toNormalizedValue(DrumBar::Genre::Funk), 2.0 / 9.0);        // 2/9
    EXPECT_DOUBLE_EQ(GenreMapper::toNormalizedValue(DrumBar::Genre::Jazz), 3.0 / 9.0);        // 3/9
    EXPECT_DOUBLE_EQ(GenreMapper::toNormalizedValue(DrumBar::Genre::HipHop), 4.0 / 9.0);      // 4/9
    EXPECT_DOUBLE_EQ(GenreMapper::toNormalizedValue(DrumBar::Genre::Afrobeat), 5.0 / 9.0);    // 5/9
    EXPECT_DOUBLE_EQ(GenreMapper::toNormalizedValue(DrumBar::Genre::NewOrleans), 6.0 / 9.0);  // 6/9
    EXPECT_DOUBLE_EQ(GenreMapper::toNormalizedValue(DrumBar::Genre::Afrocuban), 7.0 / 9.0);   // 7/9
    EXPECT_DOUBLE_EQ(GenreMapper::toNormalizedValue(DrumBar::Genre::Other), 8.0 / 9.0);       // 8/9
    EXPECT_DOUBLE_EQ(GenreMapper::toNormalizedValue(DrumBar::Genre::Uncertain), 1.0);         // 9/9
}

//------------------------------------------------------------------------
// GenreMapper::fromIndex Tests
//------------------------------------------------------------------------
TEST(GenreMapperTest, FromIndex_MapsCorrectly) {
    EXPECT_EQ(GenreMapper::fromIndex(0), DrumBar::Genre::Rock);
    EXPECT_EQ(GenreMapper::fromIndex(1), DrumBar::Genre::Latin);
    EXPECT_EQ(GenreMapper::fromIndex(2), DrumBar::Genre::Funk);
    EXPECT_EQ(GenreMapper::fromIndex(3), DrumBar::Genre::Jazz);
    EXPECT_EQ(GenreMapper::fromIndex(4), DrumBar::Genre::HipHop);
    EXPECT_EQ(GenreMapper::fromIndex(5), DrumBar::Genre::Afrobeat);
    EXPECT_EQ(GenreMapper::fromIndex(6), DrumBar::Genre::NewOrleans);
    EXPECT_EQ(GenreMapper::fromIndex(7), DrumBar::Genre::Afrocuban);
    EXPECT_EQ(GenreMapper::fromIndex(8), DrumBar::Genre::Other);
    EXPECT_EQ(GenreMapper::fromIndex(9), DrumBar::Genre::Uncertain);
}

TEST(GenreMapperTest, FromIndex_InvalidIndex_ReturnsRock) {
    EXPECT_EQ(GenreMapper::fromIndex(-1), DrumBar::Genre::Rock);
    EXPECT_EQ(GenreMapper::fromIndex(10), DrumBar::Genre::Rock);
    EXPECT_EQ(GenreMapper::fromIndex(100), DrumBar::Genre::Rock);
}

//------------------------------------------------------------------------
// GenreMapper::toIndex Tests
//------------------------------------------------------------------------
TEST(GenreMapperTest, ToIndex_ReturnsCorrectValues) {
    EXPECT_EQ(GenreMapper::toIndex(DrumBar::Genre::Rock), 0);
    EXPECT_EQ(GenreMapper::toIndex(DrumBar::Genre::Latin), 1);
    EXPECT_EQ(GenreMapper::toIndex(DrumBar::Genre::Funk), 2);
    EXPECT_EQ(GenreMapper::toIndex(DrumBar::Genre::Jazz), 3);
    EXPECT_EQ(GenreMapper::toIndex(DrumBar::Genre::HipHop), 4);
    EXPECT_EQ(GenreMapper::toIndex(DrumBar::Genre::Afrobeat), 5);
    EXPECT_EQ(GenreMapper::toIndex(DrumBar::Genre::NewOrleans), 6);
    EXPECT_EQ(GenreMapper::toIndex(DrumBar::Genre::Afrocuban), 7);
    EXPECT_EQ(GenreMapper::toIndex(DrumBar::Genre::Other), 8);
    EXPECT_EQ(GenreMapper::toIndex(DrumBar::Genre::Uncertain), 9);
}

//------------------------------------------------------------------------
// Constexpr Tests
//------------------------------------------------------------------------
TEST(GenreMapperTest, Functions_AreConstexpr) {
    // Verify all functions can be used at compile time
    constexpr const char* genreStr = GenreMapper::toGenreString(DrumBar::Genre::Rock);
    constexpr const char* displayStr = GenreMapper::toDisplayString(DrumBar::Genre::Rock);
    constexpr DrumBar::Genre fromNorm = GenreMapper::fromNormalizedValue(0.5);
    constexpr double toNorm = GenreMapper::toNormalizedValue(DrumBar::Genre::Rock);
    constexpr DrumBar::Genre fromIdx = GenreMapper::fromIndex(3);
    constexpr int toIdx = GenreMapper::toIndex(DrumBar::Genre::Jazz);

    EXPECT_STREQ(genreStr, "rock");
    EXPECT_STREQ(displayStr, "Rock");
    EXPECT_EQ(fromNorm, DrumBar::Genre::Afrobeat);  // 0.5 * 10 = 5.0 -> index 5
    EXPECT_DOUBLE_EQ(toNorm, 0.0);
    EXPECT_EQ(fromIdx, DrumBar::Genre::Jazz);
    EXPECT_EQ(toIdx, 3);
}

//------------------------------------------------------------------------
// Constants Tests
//------------------------------------------------------------------------
TEST(GenreMapperTest, Constants_AreCorrect) {
    EXPECT_EQ(GenreMapper::kNumGenres, 10);
    EXPECT_EQ(DrumBar::kNumGenres, 10);
}

//------------------------------------------------------------------------
// StyleMapper Alias Test
//------------------------------------------------------------------------
TEST(GenreMapperTest, StyleMapperAlias_WorksCorrectly) {
    // Verify StyleMapper is an alias for GenreMapper (backward compatibility)
    EXPECT_EQ(StyleMapper::kNumGenres, GenreMapper::kNumGenres);
    EXPECT_STREQ(StyleMapper::toGenreString(DrumBar::Genre::Rock),
                 GenreMapper::toGenreString(DrumBar::Genre::Rock));
}

}  // namespace test
}  // namespace JKDigital
