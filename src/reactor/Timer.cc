/**
 * lhqvq
 *
 *
 */

#include "src/reactor/Timer.h"

using lhqvq::Timer;

lhqvq::AtomicInt64 Timer::count_(0);

void Timer::Restart(Timestamp now) {
    if (IsRepeated()) {
        expiration_ = now + Timestamp(interval_);
    } else {
        expiration_ = Timestamp::Invalid();
    }
}