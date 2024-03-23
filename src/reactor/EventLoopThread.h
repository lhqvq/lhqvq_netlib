/**
 * lhqvq
 *
 *
 */

#ifndef __EVENTLOOPTHREAD_H__
#define __EVENTLOOPTHREAD_H__

#include <functional>
#include <string>

#include <src/thread/CountDownLatch.h>
#include "src/reactor/EventLoop.h"
#include "src/thread/Thread.h"
#include "src/utils/Uncopyable.h"

namespace lhqvq {

// loop子线程. 析构时会等待线程结束.
// 只能开启一次, 不能重复使用.
class EventLoopThread : public Uncopyable {
public:
    using InitCallback = std::function<void ()>;

    EventLoopThread(const InitCallback& cb = InitCallback(),
                  const std::string& name = std::string());
    ~EventLoopThread();
    // 开启loop线程. 要确保只调用一次 StartLoop().
    EventLoop* StartLoop();

private:
    void LoopInThread();

    CountDownLatch latch_;
    Thread thread_;  // loop子线程.
    EventLoop* loop_;  // 子线程的loop指针.
    // 子线程进入loop()之前要执行的初始化操作.
    InitCallback initCallback_;
};


} // namespace lhqvq

#endif // __EVENTLOOPTHREAD_H__