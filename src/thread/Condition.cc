/**
 * lhqvq
 *
 *
 */

#include "src/thread/Condition.h"

#include <errno.h>
#include <pthread.h>
#include <sys/time.h>
#include <cstdint>

using lhqvq::Condition;

bool Condition::WaitForSeconds(double seconds) {
    assert(lock_.LockedByThisThread());
    const int64_t kNanosecondsPerSecond = 1000 * 1000 * 1000;

    struct timespec ts;
    ::clock_gettime(CLOCK_REALTIME, &ts); // 获取绝对时间.

    int64_t nanoseconds =
        static_cast<int64_t>(seconds * kNanosecondsPerSecond) + ts.tv_nsec;
    ts.tv_sec +=
        static_cast<decltype(ts.tv_sec)>(nanoseconds / kNanosecondsPerSecond);
    ts.tv_nsec =
        static_cast<decltype(ts.tv_nsec)>(nanoseconds % kNanosecondsPerSecond);

    // 手动修改持有者.
    lock_.UnassignHolder();
    // pthread_cond_timedwait 第三个参数要求的是绝对时间.
    int ret = ::pthread_cond_timedwait(&condition_, lock_.GetPthreadMutex(), &ts);
    lock_.AssignHolder();

    return ret == ETIMEDOUT;
}