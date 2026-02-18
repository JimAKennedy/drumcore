//------------------------------------------------------------------------
// Copyright(c) 2025-2026 JK Digital.
// SPDX-License-Identifier: Apache-2.0
// Shared constants for drum pattern processing.
//------------------------------------------------------------------------

#pragma once

#include <cstdint>

namespace JKDigital {
namespace Constants {

//------------------------------------------------------------------------
// Grid Dimensions
//------------------------------------------------------------------------

/** Number of instrument tracks in the drum grid */
constexpr int kNumInstruments = 10;

/** Number of steps per bar (32nd note resolution) */
constexpr int kStepsPerBar = 32;

/** Beats per step (constant for all time signatures) */
constexpr double kBeatsPerStep = 0.125;

//------------------------------------------------------------------------
// MIDI Note Timing
//------------------------------------------------------------------------

/** Duration of each note in seconds (50ms) */
constexpr double kNoteDurationSeconds = 0.050;

//------------------------------------------------------------------------
// Tempo Constraints
//------------------------------------------------------------------------

/** Minimum tempo in BPM */
constexpr double kMinTempo = 60.0;

/** Maximum tempo in BPM */
constexpr double kMaxTempo = 300.0;

/** Default tempo in BPM */
constexpr double kDefaultTempo = 120.0;

//------------------------------------------------------------------------
// Velocity Modifiers
//------------------------------------------------------------------------

/** Velocity multiplier for ghost notes (60% of normal) */
constexpr float kGhostVelocityMultiplier = 0.6f;

/** Velocity multiplier for accented notes (120% of normal) */
constexpr float kAccentVelocityMultiplier = 1.2f;

/** Minimum MIDI velocity */
constexpr int32_t kMinVelocity = 1;

/** Maximum MIDI velocity */
constexpr int32_t kMaxVelocity = 127;

//------------------------------------------------------------------------
// Timing Humanization
//------------------------------------------------------------------------

/** Maximum timing offset in milliseconds for humanization */
constexpr float kMaxTimingOffsetMs = 20.0f;

/** Minimum timing offset in milliseconds for humanization */
constexpr float kMinTimingOffsetMs = -20.0f;

//------------------------------------------------------------------------
// Pattern Length Constraints
//------------------------------------------------------------------------

/** Minimum number of bars in a pattern */
constexpr int32_t kMinPatternLength = 1;

/** Maximum number of bars in a pattern */
constexpr int32_t kMaxPatternLength = 16;

/** Default pattern length in bars */
constexpr int32_t kDefaultPatternLength = 8;

}  // namespace Constants
}  // namespace JKDigital
