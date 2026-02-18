//------------------------------------------------------------------------
// Copyright(c) 2025-2026 JK Digital.
// SPDX-License-Identifier: Apache-2.0
//------------------------------------------------------------------------

#include <drumcore/drumgrid.h>
#include <gtest/gtest.h>

using namespace JKDigital;

TEST(DrumStep, DefaultConstruction_IsSilent) {
    DrumStep step;
    EXPECT_FLOAT_EQ(step.velocity, 0.0f);
    EXPECT_FLOAT_EQ(step.timingOffsetMs, 0.0f);
    EXPECT_EQ(step.flags, 0);
    EXPECT_FALSE(step.hasNote());
}

TEST(DrumStep, ParameterizedConstruction) {
    DrumStep step(0.8f, -5.0f, DrumStep::FLAG_ACCENT);
    EXPECT_FLOAT_EQ(step.velocity, 0.8f);
    EXPECT_FLOAT_EQ(step.timingOffsetMs, -5.0f);
    EXPECT_TRUE(step.isAccent());
    EXPECT_FALSE(step.isGhost());
    EXPECT_TRUE(step.hasNote());
}

TEST(DrumStep, GhostFlag) {
    DrumStep step;
    step.setGhost(true);
    EXPECT_TRUE(step.isGhost());
    step.setGhost(false);
    EXPECT_FALSE(step.isGhost());
}

TEST(DrumStep, AccentFlag) {
    DrumStep step;
    step.setAccent(true);
    EXPECT_TRUE(step.isAccent());
    step.setAccent(false);
    EXPECT_FALSE(step.isAccent());
}

TEST(DrumStep, FillCandidateFlag) {
    DrumStep step;
    step.setFillCandidate(true);
    EXPECT_TRUE(step.isFillCandidate());
    step.setFillCandidate(false);
    EXPECT_FALSE(step.isFillCandidate());
}

TEST(DrumStep, MultipleFlags) {
    DrumStep step;
    step.setGhost(true);
    step.setAccent(true);
    EXPECT_TRUE(step.isGhost());
    EXPECT_TRUE(step.isAccent());
    step.setGhost(false);
    EXPECT_FALSE(step.isGhost());
    EXPECT_TRUE(step.isAccent());
}

TEST(DrumStep, Clear) {
    DrumStep step(0.9f, 10.0f, DrumStep::FLAG_GHOST | DrumStep::FLAG_ACCENT);
    step.clear();
    EXPECT_FLOAT_EQ(step.velocity, 0.0f);
    EXPECT_FLOAT_EQ(step.timingOffsetMs, 0.0f);
    EXPECT_EQ(step.flags, 0);
}

TEST(DrumBar, DefaultConstruction_IsEmpty) {
    DrumBar bar;
    EXPECT_FALSE(bar.hasNotes());
    EXPECT_EQ(bar.genre, DrumBar::Genre::Rock);
    EXPECT_EQ(bar.role, DrumBar::Role::MainGroove);
    EXPECT_EQ(bar.barIndex, -1);
}

TEST(DrumBar, SetAndGetStep) {
    DrumBar bar;
    bar.getStep(0, 0).velocity = 0.9f;
    bar.getStep(1, 16).velocity = 0.7f;
    EXPECT_TRUE(bar.hasNotes());
    EXPECT_FLOAT_EQ(bar.getStep(0, 0).velocity, 0.9f);
    EXPECT_FLOAT_EQ(bar.getStep(1, 16).velocity, 0.7f);
}

TEST(DrumBar, Clear_RemovesAllNotes) {
    DrumBar bar;
    bar.getStep(0, 0).velocity = 1.0f;
    bar.getStep(5, 20).velocity = 0.5f;
    EXPECT_TRUE(bar.hasNotes());
    bar.clear();
    EXPECT_FALSE(bar.hasNotes());
}

TEST(DrumBar, Constants) {
    EXPECT_EQ(DrumBar::NUM_INSTRUMENTS, 10);
    EXPECT_EQ(DrumBar::STEPS_PER_BAR, 32);
}

TEST(DrumPatternBuffer, InitiallyEmpty) {
    DrumPatternBuffer buffer;
    EXPECT_TRUE(buffer.isEmpty());
    EXPECT_FALSE(buffer.isFull());
    EXPECT_EQ(buffer.size(), 0u);
}

TEST(DrumPatternBuffer, PushAndPop) {
    DrumPatternBuffer buffer;
    DrumBar bar;
    bar.getStep(0, 0).velocity = 0.8f;

    EXPECT_TRUE(buffer.push(bar));
    EXPECT_EQ(buffer.size(), 1u);

    DrumBar result;
    EXPECT_TRUE(buffer.pop(result));
    EXPECT_FLOAT_EQ(result.getStep(0, 0).velocity, 0.8f);
    EXPECT_TRUE(buffer.isEmpty());
}

TEST(DrumPatternBuffer, PopFromEmpty_ReturnsFalse) {
    DrumPatternBuffer buffer;
    DrumBar bar;
    EXPECT_FALSE(buffer.pop(bar));
}

TEST(DrumPatternBuffer, FillToCapacity) {
    DrumPatternBuffer buffer;
    DrumBar bar;

    // Capacity is 8 but one slot is reserved, so max 7 pushes
    for (size_t i = 0; i < DrumPatternBuffer::CAPACITY - 1; ++i) {
        EXPECT_TRUE(buffer.push(bar));
    }
    EXPECT_TRUE(buffer.isFull());
    EXPECT_FALSE(buffer.push(bar));
}

TEST(DrumPatternBuffer, Reset) {
    DrumPatternBuffer buffer;
    DrumBar bar;
    buffer.push(bar);
    buffer.push(bar);
    buffer.reset();
    EXPECT_TRUE(buffer.isEmpty());
    EXPECT_EQ(buffer.size(), 0u);
}
