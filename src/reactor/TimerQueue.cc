/**
 * lhqvq
 *
 *
 */
#include "src/reactor/TimerQueue.h"

#include <unistd.h>
#include <sys/timerfd.h>

#include <cassert>
#include <cstdint>
#include <cstring>
#include <functional>
#include <utility>

#include "src/reactor/EventLoop.h"
#include "src/logger/Logger.h"

namespace lhqvq {
namespace detail {

// 创建并检查 timerfd.
int CreateTimerfd() {
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC | TFD_NONBLOCK);
    if (timerfd < 0) {
        LOG_SYSFATAL << "timerfd_create() error.";
    }
    return timerfd;
}

// 有事件到来时读 timerfd.
void ReadTimerfd(int timerfd) {
    uint64_t count;  // 从 timerfd 读到的超时次数.
    ssize_t n = ::read(timerfd, &count, sizeof(count));
    LOG_TRACE << "TimerQueue::HandleRead(), count = " << count;
    if (n != sizeof(count)) {
        LOG_ERROR << "TimerQueue::HandleRead(): bytes read is not "
              << sizeof(count);
    }
}

// 更新 timerfd 的到期时间.
void UpdateTimerfd(int timerfd, Timestamp expiration) {
    struct itimerspec newVal;
    ::memset(&newVal, 0, sizeof(newVal));
    Timestamp now = Timestamp::Now();
    if (now < expiration) {
        // 不需要设置 newVal.it_interval, 重复运行的功能由 TimerQueue 实现.
        newVal.it_value = expiration - Timestamp::Now();
    } else {  // 已经过期了, 在 timerfd 中设置为1微秒后过期.
        LOG_WARN << "added timer has expirated.";
        newVal.it_value.tv_nsec = 1000;
    }

    // flag 默认为 0, 表明 it_value 使用相对时间.
    int ret = ::timerfd_settime(timerfd, 0, &newVal, nullptr);
    if (ret < 0) {
        LOG_SYSERROR << "timerfd_settime() error.";
    }
}

} // namespace detail
} // namespace lhqvq