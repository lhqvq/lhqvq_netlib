/**
 * lhqvq
 *
 *
 */

#include "src/thread/CurrentThread.h"

#include <unistd.h>
#include <sys/syscall.h>
#include <time.h>
#include <type_traits>

// 确保 pid_t 同 int 类型
static_assert(std::is_same<int, pid_t>::value, "pid_t should be int");

namespace lhqvq {
namespace current_thread {
namespace detail {


class MainThreadInitializer {
public:
    MainThreadInitializer() {
        current_thread::Tid();
        current_thread::t_name = "MainThread";
    }
};
// 一个全局变量, 主线程调用它的构造函数时完成初始信息设置.
MainThreadInitializer gInitializer;

} // namespace detail

__thread int t_tid = 0; // 0代表没有缓存.
__thread const char *t_name = "unknown";

} // namespace current_thread
} // namespace lhqvq

// 显示地指出要缓存tid.
int lhqvq::current_thread::Tid() {
  if (t_tid == 0)
    t_tid = static_cast<pid_t>(::syscall(SYS_gettid));
  return t_tid;
}

const char* lhqvq::current_thread::Name() {
  return t_name;
}

bool lhqvq::current_thread::IsMainThread() {
  return Tid() == ::getpid();
}

void lhqvq::current_thread::SleepUsec(int64_t usec) {
  struct timespec ts;
  ts.tv_sec = static_cast<decltype(ts.tv_sec)>(usec / 1000000);
  ts.tv_nsec = static_cast<decltype(ts.tv_nsec)>(usec % 1000000 * 1000);
  ::nanosleep(&ts, nullptr);
}
