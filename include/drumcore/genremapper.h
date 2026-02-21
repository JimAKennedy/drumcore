//------------------------------------------------------------------------
// Copyright(c) 2025-2026 JK Digital.
// SPDX-License-Identifier: Apache-2.0
// Genre mapping utilities for the 10-genre drum pattern system.
//------------------------------------------------------------------------

#pragma once

#include <drumcore/drumgrid.h>

namespace JKDigital {

//------------------------------------------------------------------------
// GenreMapper - Centralized genre mapping utilities for 10-genre system
//------------------------------------------------------------------------
/**
 * @brief Utility namespace for genre mapping operations.
 *
 * This namespace provides constexpr functions for mapping between:
 * - DrumBar::Genre enum (pattern generation and UI)
 * - Genre strings (PlannerEngine, model loading)
 * - Normalized parameter values (0.0-1.0 for VST3 parameters)
 *
 * All functions are constexpr and real-time safe (no allocations).
 *
 * Genre enum values (10 total, matching VAE training):
 * - Rock = 0
 * - Latin = 1
 * - Funk = 2
 * - Jazz = 3
 * - HipHop = 4
 * - Afrobeat = 5
 * - NewOrleans = 6
 * - Afrocuban = 7
 * - Other = 8
 * - Uncertain = 9 (novel/hybrid patterns)
 */
namespace GenreMapper {

/** Number of genres (matches DrumBar::kNumGenres) */
constexpr int kNumGenres = DrumBar::kNumGenres;

//------------------------------------------------------------------------
// String conversions (for PlannerEngine, model loading, and logging)
//------------------------------------------------------------------------

/**
 * @brief Get genre string for ML models from DrumBar::Genre.
 *
 * Returns static const char* strings that are safe for real-time use.
 * These strings match the genre labels used in VAE training.
 *
 * @param genre The DrumBar::Genre to convert
 * @return Genre string (lowercase, no spaces)
 */
constexpr const char* toGenreString(DrumBar::Genre genre) {
    switch (genre) {
        case DrumBar::Genre::Rock:
            return "rock";
        case DrumBar::Genre::Latin:
            return "latin";
        case DrumBar::Genre::Funk:
            return "funk";
        case DrumBar::Genre::Jazz:
            return "jazz";
        case DrumBar::Genre::HipHop:
            return "hiphop";
        case DrumBar::Genre::Afrobeat:
            return "afrobeat";
        case DrumBar::Genre::NewOrleans:
            return "neworleans";
        case DrumBar::Genre::Afrocuban:
            return "afrocuban";
        case DrumBar::Genre::Other:
            return "other";
        case DrumBar::Genre::Uncertain:
            return "uncertain";
        default:
            return "rock";
    }
}

/**
 * @brief Get display string for DrumBar::Genre.
 *
 * Returns static const char* strings for UI display.
 * These are user-friendly display names.
 *
 * @param genre The DrumBar::Genre to convert
 * @return Display string (title case, human readable)
 */
constexpr const char* toDisplayString(DrumBar::Genre genre) {
    switch (genre) {
        case DrumBar::Genre::Rock:
            return "Rock";
        case DrumBar::Genre::Latin:
            return "Latin";
        case DrumBar::Genre::Funk:
            return "Funk";
        case DrumBar::Genre::Jazz:
            return "Jazz";
        case DrumBar::Genre::HipHop:
            return "HipHop";
        case DrumBar::Genre::Afrobeat:
            return "Afrobeat";
        case DrumBar::Genre::NewOrleans:
            return "New Orleans";
        case DrumBar::Genre::Afrocuban:
            return "Afrocuban";
        case DrumBar::Genre::Other:
            return "Other";
        case DrumBar::Genre::Uncertain:
            return "Uncertain";
        default:
            return "Rock";
    }
}

//------------------------------------------------------------------------
// Parameter normalization (for VST3 parameters)
//------------------------------------------------------------------------

/**
 * @brief Convert normalized parameter value (0.0-1.0) to DrumBar::Genre.
 *
 * Maps the continuous 0.0-1.0 parameter range to discrete genre values.
 * Values are distributed evenly across the 10 genres.
 *
 * @param value Normalized value from parameter (0.0-1.0)
 * @return Corresponding DrumBar::Genre
 */
constexpr DrumBar::Genre fromNormalizedValue(double value) {
    int genreIndex = static_cast<int>(value * static_cast<double>(kNumGenres));
    if (genreIndex < 0) {
        genreIndex = 0;
    } else if (genreIndex >= kNumGenres) {
        genreIndex = kNumGenres - 1;  // Clamp for value == 1.0
    }
    return static_cast<DrumBar::Genre>(genreIndex);
}

/**
 * @brief Convert DrumBar::Genre to normalized parameter value (0.0-1.0).
 *
 * Maps discrete genre values to the continuous 0.0-1.0 parameter range.
 *
 * @param genre The DrumBar::Genre to convert
 * @return Normalized value for parameter (0.0-1.0)
 */
constexpr double toNormalizedValue(DrumBar::Genre genre) {
    return static_cast<double>(static_cast<int>(genre)) / static_cast<double>(kNumGenres - 1);
}

/**
 * @brief Convert integer genre index to DrumBar::Genre with bounds checking.
 *
 * @param index Genre index (0-9)
 * @return Corresponding DrumBar::Genre, or Genre::Rock if out of bounds
 */
constexpr DrumBar::Genre fromIndex(int index) {
    if (index < 0 || index >= kNumGenres) {
        return DrumBar::Genre::Rock;  // Safe default
    }
    return static_cast<DrumBar::Genre>(index);
}

/**
 * @brief Convert DrumBar::Genre to integer index.
 *
 * @param genre The DrumBar::Genre to convert
 * @return Integer index (0-9)
 */
constexpr int toIndex(DrumBar::Genre genre) {
    return static_cast<int>(genre);
}

}  // namespace GenreMapper

// Keep StyleMapper as an alias for backward compatibility during transition
namespace StyleMapper = GenreMapper;

}  // namespace JKDigital
