//------------------------------------------------------------------------
// Copyright(c) 2025-2026 JK Digital.
// SPDX-License-Identifier: Apache-2.0
//------------------------------------------------------------------------

#include <drumcore/denormalguard.h>
#include <gtest/gtest.h>

namespace JKDigital {

#if defined(__aarch64__) || defined(_M_ARM64)

static uint64_t getFPCR() {
    uint64_t fpcr;
    __asm__ __volatile__("mrs %0, fpcr" : "=r"(fpcr));
    return fpcr;
}

static constexpr uint64_t kFZBit = 1ULL << 24;

TEST(DenormalGuard, SetsFZOnARM) {
    uint64_t before = getFPCR();
    {
        DenormalGuard guard;
        uint64_t during = getFPCR();
        EXPECT_TRUE(during & kFZBit) << "FZ bit should be set inside guard scope";
    }
    uint64_t after = getFPCR();
    EXPECT_EQ(before, after) << "FPCR should be restored after guard destruction";
}

TEST(DenormalGuard, NestedGuards) {
    uint64_t original = getFPCR();
    {
        DenormalGuard outer;
        uint64_t afterOuter = getFPCR();
        EXPECT_TRUE(afterOuter & kFZBit);
        {
            DenormalGuard inner;
            uint64_t afterInner = getFPCR();
            EXPECT_TRUE(afterInner & kFZBit);
        }
        uint64_t afterInnerDestroyed = getFPCR();
        EXPECT_TRUE(afterInnerDestroyed & kFZBit)
            << "Outer guard's FZ should still be active after inner guard dies";
    }
    uint64_t afterAll = getFPCR();
    EXPECT_EQ(original, afterAll) << "Original FPCR should be fully restored";
}

#elif defined(__SSE__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)

static constexpr unsigned kFTZ = 1u << 15;
static constexpr unsigned kDAZ = 1u << 6;

TEST(DenormalGuard, SetsFTZAndDAZOnX86) {
    unsigned before = _mm_getcsr();
    {
        DenormalGuard guard;
        unsigned during = _mm_getcsr();
        EXPECT_TRUE(during & kFTZ) << "FTZ bit should be set inside guard scope";
        EXPECT_TRUE(during & kDAZ) << "DAZ bit should be set inside guard scope";
    }
    unsigned after = _mm_getcsr();
    EXPECT_EQ(before, after) << "MXCSR should be restored after guard destruction";
}

TEST(DenormalGuard, NestedGuards) {
    unsigned original = _mm_getcsr();
    // Clear FTZ/DAZ so we can observe the guard setting them
    _mm_setcsr(original & ~(kFTZ | kDAZ));
    unsigned clean = _mm_getcsr();
    {
        DenormalGuard outer;
        unsigned afterOuter = _mm_getcsr();
        EXPECT_TRUE(afterOuter & kFTZ);
        EXPECT_TRUE(afterOuter & kDAZ);
        {
            DenormalGuard inner;
            EXPECT_TRUE(_mm_getcsr() & kFTZ);
        }
        unsigned afterInnerDestroyed = _mm_getcsr();
        EXPECT_TRUE(afterInnerDestroyed & kFTZ)
            << "Outer guard's FTZ should still be active";
    }
    unsigned afterAll = _mm_getcsr();
    EXPECT_EQ(clean, afterAll) << "Original MXCSR should be fully restored";
    // Restore the actual original
    _mm_setcsr(original);
}

#else

TEST(DenormalGuard, NoOpOnUnsupportedPlatform) {
    DenormalGuard guard;
    SUCCEED() << "DenormalGuard compiles and runs as no-op on this platform";
}

#endif

TEST(DenormalGuard, AliasExists) {
    static_assert(std::is_same<DenormalGuard, ScopedDenormalDisable>::value,
                  "ScopedDenormalDisable should be an alias for DenormalGuard");
}

} // namespace JKDigital
