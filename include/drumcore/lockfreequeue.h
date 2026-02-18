//------------------------------------------------------------------------
// Copyright(c) 2025-2026 JK Digital.
// SPDX-License-Identifier: Apache-2.0
// Lock-free SPSC queue template.
//------------------------------------------------------------------------

#pragma once

#include <atomic>
#include <cstddef>

namespace JKDigital {

/**
 * Lock-free single-producer single-consumer (SPSC) queue.
 *
 * Real-time safe: no allocations, no blocking.
 *
 * @tparam T Type of elements (must be copyable)
 * @tparam Capacity Number of slots (must be power-of-2)
 */
template <typename T, size_t Capacity = 16> class LockFreeQueue {
    static_assert((Capacity & (Capacity - 1)) == 0, "Capacity must be a power of 2");
    static_assert(Capacity > 0, "Capacity must be greater than 0");

  public:
    static constexpr size_t CAPACITY = Capacity;

    /** Constructor - initializes empty queue. */
    LockFreeQueue() : head_(0), tail_(0) {}

    /** Push an item into the queue (producer). */
    bool push(const T& item) {
        const size_t currentHead = head_.load(std::memory_order_relaxed);
        const size_t nextHead = (currentHead + 1) & (Capacity - 1);

        if (nextHead == tail_.load(std::memory_order_acquire)) {
            return false;
        }

        buffer_[currentHead] = item;
        head_.store(nextHead, std::memory_order_release);
        return true;
    }

    /** Pop an item from the queue (consumer). */
    bool pop(T& item) {
        const size_t currentTail = tail_.load(std::memory_order_relaxed);

        if (currentTail == head_.load(std::memory_order_acquire)) {
            return false;
        }

        item = buffer_[currentTail];
        const size_t nextTail = (currentTail + 1) & (Capacity - 1);
        tail_.store(nextTail, std::memory_order_release);
        return true;
    }

    /** Check if the queue is empty. */
    bool isEmpty() const {
        return tail_.load(std::memory_order_acquire) == head_.load(std::memory_order_acquire);
    }

    /** Check if the queue is full. */
    bool isFull() const {
        const size_t currentHead = head_.load(std::memory_order_acquire);
        const size_t nextHead = (currentHead + 1) & (Capacity - 1);
        return nextHead == tail_.load(std::memory_order_acquire);
    }

    /** Get the current number of items in the queue. */
    size_t size() const {
        const size_t h = head_.load(std::memory_order_acquire);
        const size_t t = tail_.load(std::memory_order_acquire);
        return (h >= t) ? (h - t) : (Capacity - t + h);
    }

    /** Reset the queue to empty state. NOT thread-safe. */
    void reset() {
        head_.store(0, std::memory_order_release);
        tail_.store(0, std::memory_order_release);
    }

  private:
    T buffer_[Capacity];
    std::atomic<size_t> head_;
    std::atomic<size_t> tail_;

    LockFreeQueue(const LockFreeQueue&) = delete;
    LockFreeQueue& operator=(const LockFreeQueue&) = delete;
};

}  // namespace JKDigital
