#include <drumcore/drummapping.h>
#include <gtest/gtest.h>

using namespace JKDigital;

TEST(GMDrumMap, GetNote_ValidIndices) {
    EXPECT_EQ(GMDrumMap::getNote(0), 36);  // Kick
    EXPECT_EQ(GMDrumMap::getNote(1), 38);  // Snare
    EXPECT_EQ(GMDrumMap::getNote(2), 42);  // Closed HH
    EXPECT_EQ(GMDrumMap::getNote(3), 46);  // Open HH
    EXPECT_EQ(GMDrumMap::getNote(4), 37);  // Rim
    EXPECT_EQ(GMDrumMap::getNote(5), 45);  // Low Tom
    EXPECT_EQ(GMDrumMap::getNote(6), 50);  // High Tom
    EXPECT_EQ(GMDrumMap::getNote(7), 49);  // Crash
    EXPECT_EQ(GMDrumMap::getNote(8), 51);  // Ride
    EXPECT_EQ(GMDrumMap::getNote(9), 56);  // Percussion
}

TEST(GMDrumMap, GetNote_OutOfRange_ReturnsKick) {
    EXPECT_EQ(GMDrumMap::getNote(-1), GMDrumMap::KICK);
    EXPECT_EQ(GMDrumMap::getNote(10), GMDrumMap::KICK);
    EXPECT_EQ(GMDrumMap::getNote(100), GMDrumMap::KICK);
}

TEST(GMDrumMap, ToMidiVelocity_NormalRange) {
    EXPECT_EQ(GMDrumMap::toMidiVelocity(1.0f), 127);
    EXPECT_EQ(GMDrumMap::toMidiVelocity(0.5f), 63);
    EXPECT_GE(GMDrumMap::toMidiVelocity(0.01f), 1);
}

TEST(GMDrumMap, ToMidiVelocity_ZeroOrNegative) {
    EXPECT_EQ(GMDrumMap::toMidiVelocity(0.0f), 0);
    EXPECT_EQ(GMDrumMap::toMidiVelocity(-0.5f), 0);
}

TEST(GMDrumMap, ToMidiVelocity_Clamping) {
    EXPECT_EQ(GMDrumMap::toMidiVelocity(2.0f), 127);
}

TEST(GMDrumMap, NamedConstants) {
    EXPECT_EQ(GMDrumMap::KICK, 36);
    EXPECT_EQ(GMDrumMap::SNARE, 38);
    EXPECT_EQ(GMDrumMap::CLOSED_HH, 42);
    EXPECT_EQ(GMDrumMap::OPEN_HH, 46);
    EXPECT_EQ(GMDrumMap::RIM, 37);
    EXPECT_EQ(GMDrumMap::LOW_TOM, 45);
    EXPECT_EQ(GMDrumMap::HIGH_TOM, 50);
    EXPECT_EQ(GMDrumMap::CRASH, 49);
    EXPECT_EQ(GMDrumMap::RIDE, 51);
    EXPECT_EQ(GMDrumMap::PERCUSSION, 56);
    EXPECT_EQ(GMDrumMap::MID_TOM, 47);
}
