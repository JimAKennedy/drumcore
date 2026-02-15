//------------------------------------------------------------------------
// Copyright(c) 2025-2026 JK Digital.
// Time signature definitions and utilities.
//------------------------------------------------------------------------

#pragma once

namespace JKDigital {

/** Supported time signatures. */
enum class TimeSignature {
    k4_4 = 0,
    k3_4 = 1,
    k6_8 = 2,
    k5_4 = 3,
    k7_4 = 4,
    k7_8 = 5,
    k12_8 = 6
};

/**
 * Utility functions for time signature-aware step calculations.
 *
 * beatsPerStep is always 0.125 PPQ (8 steps per beat). Only the number
 * of active steps changes based on time signature.
 */
namespace TimeSignatureUtils {

/** Get the number of active steps for a time signature. */
constexpr int getActiveSteps(TimeSignature timeSig) {
    switch (timeSig) {
        case TimeSignature::k4_4:  return 32;
        case TimeSignature::k3_4:  return 24;
        case TimeSignature::k6_8:  return 24;
        case TimeSignature::k5_4:  return 32;
        case TimeSignature::k7_4:  return 32;
        case TimeSignature::k7_8:  return 28;
        case TimeSignature::k12_8: return 32;
        default:                   return 32;
    }
}

/** Get the constant beats per step value (always 0.125). */
constexpr double getBeatsPerStep() {
    return 0.125;
}

/** Get the numerator of a time signature. */
constexpr double getNumerator(TimeSignature timeSig) {
    switch (timeSig) {
        case TimeSignature::k4_4:  return 4.0;
        case TimeSignature::k3_4:  return 3.0;
        case TimeSignature::k6_8:  return 6.0;
        case TimeSignature::k5_4:  return 5.0;
        case TimeSignature::k7_4:  return 7.0;
        case TimeSignature::k7_8:  return 7.0;
        case TimeSignature::k12_8: return 12.0;
        default:                   return 4.0;
    }
}

/** Get the denominator of a time signature. */
constexpr double getDenominator(TimeSignature timeSig) {
    switch (timeSig) {
        case TimeSignature::k4_4:  return 4.0;
        case TimeSignature::k3_4:  return 4.0;
        case TimeSignature::k6_8:  return 8.0;
        case TimeSignature::k5_4:  return 4.0;
        case TimeSignature::k7_4:  return 4.0;
        case TimeSignature::k7_8:  return 8.0;
        case TimeSignature::k12_8: return 8.0;
        default:                   return 4.0;
    }
}

/** Get the number of quarter-note beats per bar (PPQ length). */
constexpr double getBeatsPerBar(TimeSignature timeSig) {
    return (getNumerator(timeSig) / getDenominator(timeSig)) * 4.0;
}

}  // namespace TimeSignatureUtils
}  // namespace JKDigital
