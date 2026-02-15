//------------------------------------------------------------------------
// Copyright(c) 2025-2026 JK Digital.
// GM drum mapping constants.
//------------------------------------------------------------------------

#pragma once

#include <cstdint>

namespace JKDigital {

/**
 * General MIDI (GM) standard drum note mappings for 10-instrument grid.
 *
 * Grid index mapping (aligned with VAE training data):
 * - 0: Kick Drum, 1: Snare, 2: Closed HH, 3: Open HH, 4: Rim,
 * - 5: Low Tom, 6: High Tom, 7: Crash, 8: Ride, 9: Percussion
 */
namespace GMDrumMap {

constexpr int32_t KICK = 36;
constexpr int32_t SNARE = 38;
constexpr int32_t CLOSED_HH = 42;
constexpr int32_t OPEN_HH = 46;
constexpr int32_t RIM = 37;
constexpr int32_t LOW_TOM = 45;
constexpr int32_t HIGH_TOM = 50;
constexpr int32_t CRASH = 49;
constexpr int32_t RIDE = 51;
constexpr int32_t PERCUSSION = 56;

/** Legacy alias for backwards compatibility. */
constexpr int32_t MID_TOM = 47;

/**
 * Map instrument index (0-9) to GM MIDI note number.
 *
 * @param instrumentIndex Instrument index in the grid (0-9)
 * @return GM MIDI note number
 */
inline int32_t getNote(int instrumentIndex) {
    constexpr int32_t NOTE_MAP[10] = {
        KICK, SNARE, CLOSED_HH, OPEN_HH, RIM,
        LOW_TOM, HIGH_TOM, CRASH, RIDE, PERCUSSION
    };

    if (instrumentIndex >= 0 && instrumentIndex < 10) {
        return NOTE_MAP[instrumentIndex];
    }
    return KICK;
}

/**
 * Convert normalized velocity (0.0-1.0) to MIDI velocity (1-127).
 *
 * @param normalizedVelocity Velocity in 0.0-1.0 range
 * @return MIDI velocity in 0-127 range (0 for silent)
 */
inline int32_t toMidiVelocity(float normalizedVelocity) {
    if (normalizedVelocity <= 0.0f) {
        return 0;
    }

    int32_t velocity = static_cast<int32_t>(normalizedVelocity * 127.0f);
    if (velocity < 1) velocity = 1;
    if (velocity > 127) velocity = 127;
    return velocity;
}

}  // namespace GMDrumMap
}  // namespace JKDigital
