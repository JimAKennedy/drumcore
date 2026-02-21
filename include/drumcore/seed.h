//------------------------------------------------------------------------
// Copyright(c) 2025-2026 JK Digital.
// SPDX-License-Identifier: Apache-2.0
// Deterministic seed management and random number generation.
//------------------------------------------------------------------------

#pragma once

#include <cstdint>

namespace JKDigital {

/**
 * Deterministic seed management using SplitMix64.
 *
 * Provides reproducible random seeds derived from a master seed,
 * transform index, and bar index. Same inputs always produce the
 * same output.
 */
namespace Seed {

/**
 * SplitMix64 hash function for seed derivation.
 *
 * @param x Input value
 * @return Hashed value
 */
inline uint64_t splitmix64(uint64_t x) {
    x += 0x9E3779B97F4A7C15ULL;
    x = (x ^ (x >> 30)) * 0xBF58476D1CE4E5B9ULL;
    x = (x ^ (x >> 27)) * 0x94D049BB133111EBULL;
    x = x ^ (x >> 31);
    return x;
}

/**
 * Derive a deterministic seed for a specific transform and bar.
 *
 * @param masterSeed The global master seed
 * @param transformIndex Index of the transform in the chain
 * @param barIndex Index of the current bar
 * @return Derived seed unique to this combination
 */
inline uint64_t deriveSeed(uint64_t masterSeed, uint32_t transformIndex, uint32_t barIndex) {
    return splitmix64(masterSeed ^ static_cast<uint64_t>(transformIndex) ^
                      (static_cast<uint64_t>(barIndex) << 32));
}

/**
 * Convert a normalized parameter value (0.0-1.0) to a seed.
 *
 * @param normalizedValue Parameter value in 0.0-1.0 range
 * @return Seed value
 */
inline uint64_t fromNormalized(double normalizedValue) {
    if (normalizedValue >= 1.0) return UINT64_MAX;
    if (normalizedValue <= 0.0) return 0;
    return static_cast<uint64_t>(normalizedValue * static_cast<double>(UINT64_MAX));
}

/**
 * Simple xorshift64 PRNG for fast random numbers from a seed.
 * Mutates the seed in place.
 *
 * @param state Mutable seed state
 * @return Next random value
 */
inline uint64_t nextRandom(uint64_t& state) {
    state ^= state << 13;
    state ^= state >> 7;
    state ^= state << 17;
    return state;
}

/**
 * Generate a random float in [0.0, 1.0) from a seed state.
 *
 * @param state Mutable seed state
 * @return Random float in [0.0, 1.0)
 */
inline float randomFloat(uint64_t& state) {
    return static_cast<float>(nextRandom(state) >> 40) / static_cast<float>(1ULL << 24);
}

}  // namespace Seed
}  // namespace JKDigital
