#include <drumcore/constants.h>
#include <gtest/gtest.h>

using namespace JKDigital;

TEST(Constants, GridDimensions) {
    EXPECT_EQ(Constants::kNumInstruments, 10);
    EXPECT_EQ(Constants::kStepsPerBar, 32);
    EXPECT_DOUBLE_EQ(Constants::kBeatsPerStep, 0.125);
}

TEST(Constants, TempoConstraints) {
    EXPECT_DOUBLE_EQ(Constants::kMinTempo, 60.0);
    EXPECT_DOUBLE_EQ(Constants::kMaxTempo, 300.0);
    EXPECT_DOUBLE_EQ(Constants::kDefaultTempo, 120.0);
    EXPECT_GT(Constants::kMaxTempo, Constants::kMinTempo);
}

TEST(Constants, VelocityModifiers) {
    EXPECT_LT(Constants::kGhostVelocityMultiplier, 1.0f);
    EXPECT_GT(Constants::kAccentVelocityMultiplier, 1.0f);
    EXPECT_EQ(Constants::kMinVelocity, 1);
    EXPECT_EQ(Constants::kMaxVelocity, 127);
}

TEST(Constants, TimingHumanization) {
    EXPECT_GT(Constants::kMaxTimingOffsetMs, 0.0f);
    EXPECT_LT(Constants::kMinTimingOffsetMs, 0.0f);
    EXPECT_FLOAT_EQ(Constants::kMaxTimingOffsetMs, -Constants::kMinTimingOffsetMs);
}

TEST(Constants, PatternLength) {
    EXPECT_EQ(Constants::kMinPatternLength, 1);
    EXPECT_EQ(Constants::kMaxPatternLength, 16);
    EXPECT_GE(Constants::kDefaultPatternLength, Constants::kMinPatternLength);
    EXPECT_LE(Constants::kDefaultPatternLength, Constants::kMaxPatternLength);
}

TEST(Constants, NoteDuration) {
    EXPECT_DOUBLE_EQ(Constants::kNoteDurationSeconds, 0.050);
}
