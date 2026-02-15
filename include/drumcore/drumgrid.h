//------------------------------------------------------------------------
// Copyright(c) 2025-2026 JK Digital.
// Drum pattern grid data structures.
//------------------------------------------------------------------------

#pragma once

#include <atomic>
#include <cstdint>
#include <cstring>

namespace JKDigital {

//------------------------------------------------------------------------
// DrumStep - represents a single step in the drum grid
//------------------------------------------------------------------------
/**
 * Represents a single step in the 10x32 drum pattern grid.
 *
 * Each step stores velocity, timing offset for humanization, and behavior flags.
 * This structure is designed to be compact and efficiently copied in real-time
 * audio contexts.
 */
struct DrumStep {
    /** Velocity of the note (0.0 = silent, 1.0 = maximum) */
    float velocity;

    /** Timing offset in milliseconds for humanization (+/-20ms) */
    float timingOffsetMs;

    /** Behavior flags (ghost, accent, fill candidate) */
    uint8_t flags;

    // Flag bit positions
    static constexpr uint8_t FLAG_GHOST = 1 << 0;
    static constexpr uint8_t FLAG_ACCENT = 1 << 1;
    static constexpr uint8_t FLAG_FILL_CANDIDATE = 1 << 2;

    /** Default constructor - initializes to silent step. */
    DrumStep() : velocity(0.0f), timingOffsetMs(0.0f), flags(0) {}

    /** Constructor with parameters. */
    DrumStep(float vel, float offset, uint8_t f) : velocity(vel), timingOffsetMs(offset), flags(f) {}

    /** Mark this step as a ghost note. */
    void setGhost(bool enabled) {
        if (enabled)
            flags |= FLAG_GHOST;
        else
            flags &= ~FLAG_GHOST;
    }

    /** Check if this step is a ghost note. */
    bool isGhost() const { return (flags & FLAG_GHOST) != 0; }

    /** Mark this step as an accented note. */
    void setAccent(bool enabled) {
        if (enabled)
            flags |= FLAG_ACCENT;
        else
            flags &= ~FLAG_ACCENT;
    }

    /** Check if this step is accented. */
    bool isAccent() const { return (flags & FLAG_ACCENT) != 0; }

    /** Mark this step as a candidate for fill patterns. */
    void setFillCandidate(bool enabled) {
        if (enabled)
            flags |= FLAG_FILL_CANDIDATE;
        else
            flags &= ~FLAG_FILL_CANDIDATE;
    }

    /** Check if this step is a fill candidate. */
    bool isFillCandidate() const { return (flags & FLAG_FILL_CANDIDATE) != 0; }

    /** Check if this step has any note data (velocity > 0). */
    bool hasNote() const { return velocity > 0.0f; }

    /** Clear all data (set to silent). */
    void clear() {
        velocity = 0.0f;
        timingOffsetMs = 0.0f;
        flags = 0;
    }
};

//------------------------------------------------------------------------
// DrumBar - represents one bar of 10x32 drum pattern grid
//------------------------------------------------------------------------
/**
 * Represents one bar of drum pattern using a 10x32 grid.
 *
 * Grid structure:
 * - 10 rows (instruments): Kick, Snare, ClosedHH, OpenHH, Rim, LowTom, HighTom, Crash, Ride, Perc
 * - 32 columns (steps): 32nd note resolution
 */
struct DrumBar {
    /** Number of instrument tracks in the grid. */
    static constexpr int NUM_INSTRUMENTS = 10;

    /** Number of steps per bar (32nd note resolution). */
    static constexpr int STEPS_PER_BAR = 32;

    /** Genre classification for pattern generation. */
    enum class Genre {
        Rock = 0,
        Latin = 1,
        Funk = 2,
        Jazz = 3,
        HipHop = 4,
        Afrobeat = 5,
        NewOrleans = 6,
        Afrocuban = 7,
        Other = 8,
        Uncertain = 9
    };

    /** Number of genres in the Genre enum. */
    static constexpr int kNumGenres = 10;

    /** Role/purpose of this bar in the pattern structure. */
    enum class Role {
        MainGroove = 0,
        Break = 1,
        Fill = 2,
        Variation = 3
    };

    /** The drum pattern grid [instrument][step]. */
    DrumStep steps[NUM_INSTRUMENTS][STEPS_PER_BAR];

    /** Genre classification of this bar. */
    Genre genre;

    /** Role of this bar in the pattern. */
    Role role;

    /** Phrase position index (0 to patternLength-1), -1 if not set. */
    int32_t barIndex;

    /** Default constructor - initializes to empty pattern. */
    DrumBar() : genre(Genre::Rock), role(Role::MainGroove), barIndex(-1) { clear(); }

    /** Clear all steps in the bar (set to silent). */
    void clear() {
        for (int i = 0; i < NUM_INSTRUMENTS; ++i) {
            for (int j = 0; j < STEPS_PER_BAR; ++j) {
                steps[i][j].clear();
            }
        }
    }

    /** Get a step at the specified instrument and position. */
    DrumStep& getStep(int instrument, int step) { return steps[instrument][step]; }

    /** Get a step at the specified instrument and position (const). */
    const DrumStep& getStep(int instrument, int step) const { return steps[instrument][step]; }

    /** Check if the bar contains any notes. */
    bool hasNotes() const {
        for (int i = 0; i < NUM_INSTRUMENTS; ++i) {
            for (int j = 0; j < STEPS_PER_BAR; ++j) {
                if (steps[i][j].hasNote()) {
                    return true;
                }
            }
        }
        return false;
    }
};

//------------------------------------------------------------------------
// DrumPatternBuffer - lock-free circular buffer for DrumBar patterns
//------------------------------------------------------------------------
/**
 * Thread-safe lock-free circular buffer for DrumBar patterns.
 * SPSC (Single Producer, Single Consumer) model.
 * Real-time safe: no allocations, no blocking.
 */
class DrumPatternBuffer {
public:
    /** Buffer capacity (number of DrumBar slots). */
    static constexpr size_t CAPACITY = 8;

    /** Constructor - initializes empty buffer. */
    DrumPatternBuffer() : head_(0), tail_(0) {
        for (size_t i = 0; i < CAPACITY; ++i) {
            buffer_[i].clear();
        }
    }

    /** Push a new DrumBar into the buffer (producer). */
    bool push(const DrumBar& bar) {
        const size_t currentHead = head_.load(std::memory_order_relaxed);
        const size_t nextHead = (currentHead + 1) % CAPACITY;

        if (nextHead == tail_.load(std::memory_order_acquire)) {
            return false;
        }

        buffer_[currentHead] = bar;
        head_.store(nextHead, std::memory_order_release);
        return true;
    }

    /** Pop a DrumBar from the buffer (consumer). */
    bool pop(DrumBar& bar) {
        const size_t currentTail = tail_.load(std::memory_order_relaxed);

        if (currentTail == head_.load(std::memory_order_acquire)) {
            return false;
        }

        bar = buffer_[currentTail];
        const size_t nextTail = (currentTail + 1) % CAPACITY;
        tail_.store(nextTail, std::memory_order_release);
        return true;
    }

    /** Check if the buffer is empty. */
    bool isEmpty() const {
        return tail_.load(std::memory_order_acquire) == head_.load(std::memory_order_acquire);
    }

    /** Check if the buffer is full. */
    bool isFull() const {
        const size_t currentHead = head_.load(std::memory_order_acquire);
        const size_t nextHead = (currentHead + 1) % CAPACITY;
        return nextHead == tail_.load(std::memory_order_acquire);
    }

    /** Get the current number of items in the buffer. */
    size_t size() const {
        const size_t currentHead = head_.load(std::memory_order_acquire);
        const size_t currentTail = tail_.load(std::memory_order_acquire);
        if (currentHead >= currentTail) {
            return currentHead - currentTail;
        } else {
            return CAPACITY - currentTail + currentHead;
        }
    }

    /** Reset the buffer to empty state. NOT thread-safe. */
    void reset() {
        head_.store(0, std::memory_order_release);
        tail_.store(0, std::memory_order_release);
        for (size_t i = 0; i < CAPACITY; ++i) {
            buffer_[i].clear();
        }
    }

private:
    DrumBar buffer_[CAPACITY];
    std::atomic<size_t> head_;
    std::atomic<size_t> tail_;

    DrumPatternBuffer(const DrumPatternBuffer&) = delete;
    DrumPatternBuffer& operator=(const DrumPatternBuffer&) = delete;
};

}  // namespace JKDigital
