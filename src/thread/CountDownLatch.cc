/**
 * lhqvq
 *
 *
 */
#include "src/thread/CountDownLatch.h"
using lhqvq::CountDownLatch;
using lhqvq::MutexLockGuard;

CountDownLatch::CountDownLatch(int count)
    : lock_(), condition_(lock_), count_(count) {}

void CountDownLatch::CountDown() {
    MutexLockGuard guard(lock_);
    --count_;
    if (count_ == 0) {
        condition_.NotifyAll();
    }
}

void CountDownLatch::Wait() {
    MutexLockGuard guard(lock_);
    while (count_ > 0) {
        condition_.Wait();
    }
}

int CountDownLatch::GetDown() const {
    MutexLockGuard guard(lock_);
    return count_;
}