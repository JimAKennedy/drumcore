//------------------------------------------------------------------------
// Copyright(c) 2025-2026 JK Digital.
// SPDX-License-Identifier: Apache-2.0
//------------------------------------------------------------------------

#include <drumcore/seed.h>
#include <gtest/gtest.h>

#include <set>

using namespace JKDigital;

TEST(Seed, Splitmix64_Deterministic) {
    EXPECT_EQ(Seed::splitmix64(0), Seed::splitmix64(0));
    EXPECT_EQ(Seed::splitmix64(42), Seed::splitmix64(42));
}

TEST(Seed, Splitmix64_DifferentInputs_DifferentOutputs) {
    EXPECT_NE(Seed::splitmix64(0), Seed::splitmix64(1));
    EXPECT_NE(Seed::splitmix64(100), Seed::splitmix64(200));
}

TEST(Seed, DeriveSeed_Deterministic) {
    uint64_t a = Seed::deriveSeed(12345, 0, 0);
    uint64_t b = Seed::deriveSeed(12345, 0, 0);
    EXPECT_EQ(a, b);
}

TEST(Seed, DeriveSeed_DifferentTransformIndex_DifferentResult) {
    uint64_t a = Seed::deriveSeed(12345, 0, 0);
    uint64_t b = Seed::deriveSeed(12345, 1, 0);
    EXPECT_NE(a, b);
}

TEST(Seed, DeriveSeed_DifferentBarIndex_DifferentResult) {
    uint64_t a = Seed::deriveSeed(12345, 0, 0);
    uint64_t b = Seed::deriveSeed(12345, 0, 1);
    EXPECT_NE(a, b);
}

TEST(Seed, DeriveSeed_DifferentMasterSeed_DifferentResult) {
    uint64_t a = Seed::deriveSeed(100, 0, 0);
    uint64_t b = Seed::deriveSeed(200, 0, 0);
    EXPECT_NE(a, b);
}

TEST(Seed, FromNormalized_ZeroReturnsZero) {
    EXPECT_EQ(Seed::fromNormalized(0.0), 0u);
}

TEST(Seed, FromNormalized_OneReturnsMax) {
    EXPECT_EQ(Seed::fromNormalized(1.0), UINT64_MAX);
}

TEST(Seed, FromNormalized_NegativeReturnsZero) {
    EXPECT_EQ(Seed::fromNormalized(-0.5), 0u);
}

TEST(Seed, FromNormalized_AboveOneReturnsMax) {
    EXPECT_EQ(Seed::fromNormalized(1.5), UINT64_MAX);
}

TEST(Seed, FromNormalized_MidValue) {
    uint64_t mid = Seed::fromNormalized(0.5);
    // Should be approximately half of UINT64_MAX
    EXPECT_GT(mid, UINT64_MAX / 4);
    EXPECT_LT(mid, (UINT64_MAX / 4) * 3);
}

TEST(Seed, NextRandom_MutatesState) {
    uint64_t state = 12345;
    uint64_t original = state;
    Seed::nextRandom(state);
    EXPECT_NE(state, original);
}

TEST(Seed, NextRandom_Deterministic) {
    uint64_t stateA = 42;
    uint64_t stateB = 42;
    EXPECT_EQ(Seed::nextRandom(stateA), Seed::nextRandom(stateB));
}

TEST(Seed, NextRandom_ProducesVariedOutput) {
    uint64_t state = 99;
    std::set<uint64_t> values;
    for (int i = 0; i < 100; ++i) {
        values.insert(Seed::nextRandom(state));
    }
    // Should produce many unique values
    EXPECT_GT(values.size(), 90u);
}

TEST(Seed, RandomFloat_InRange) {
    uint64_t state = 7777;
    for (int i = 0; i < 1000; ++i) {
        float val = Seed::randomFloat(state);
        EXPECT_GE(val, 0.0f);
        EXPECT_LT(val, 1.0f);
    }
}

TEST(Seed, RandomFloat_Deterministic) {
    uint64_t stateA = 555;
    uint64_t stateB = 555;
    EXPECT_FLOAT_EQ(Seed::randomFloat(stateA), Seed::randomFloat(stateB));
}
