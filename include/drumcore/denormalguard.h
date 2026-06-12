//------------------------------------------------------------------------
// Copyright(c) 2025-2026 JK Digital.
// SPDX-License-Identifier: Apache-2.0
// RAII denormal protection (FTZ/DAZ) for real-time audio threads.
//------------------------------------------------------------------------

#pragma once

#include <cstdint>

#if defined(__SSE__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)
#include <xmmintrin.h>
#include <pmmintrin.h>
#endif

namespace JKDigital {

/**
 * RAII guard that disables denormals (flush-to-zero + denormals-are-zero)
 * for the lifetime of the object. Restores previous FPU state on destruction.
 *
 * Use at the top of your audio process() callback:
 *   void process(float** out, int frames) {
 *       DenormalGuard guard;
 *       // ... IIR filters, feedback loops, etc.
 *   }
 */
class DenormalGuard {
  public:
    DenormalGuard() noexcept {
#if defined(__SSE__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)
        saved_ = _mm_getcsr();
        _mm_setcsr(saved_ | 0x8040); // FTZ (bit 15) | DAZ (bit 6)
#elif defined(__aarch64__) || defined(_M_ARM64)
        uint64_t fpcr;
        __asm__ __volatile__("mrs %0, fpcr" : "=r"(fpcr));
        saved_ = static_cast<uint32_t>(fpcr);
        fpcr |= (1 << 24); // FZ bit
        __asm__ __volatile__("msr fpcr, %0" ::"r"(fpcr));
#else
        saved_ = 0;
#endif
    }

    ~DenormalGuard() noexcept {
#if defined(__SSE__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)
        _mm_setcsr(saved_);
#elif defined(__aarch64__) || defined(_M_ARM64)
        uint64_t fpcr = saved_;
        __asm__ __volatile__("msr fpcr, %0" ::"r"(fpcr));
#endif
    }

    DenormalGuard(const DenormalGuard&) = delete;
    DenormalGuard& operator=(const DenormalGuard&) = delete;
    DenormalGuard(DenormalGuard&&) = delete;
    DenormalGuard& operator=(DenormalGuard&&) = delete;

  private:
    uint32_t saved_;
};

using ScopedDenormalDisable = DenormalGuard;

} // namespace JKDigital
