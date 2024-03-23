/**
 * lhqvq
 *
 *
 */
#include "src/reactor/EventLoopThread.h"

using lhqvq::EventLoop;
using lhqvq::EventLoopThread;

EventLoopThread::EventLoopThread(const InitCallback &cb,
                                 const std::string &name)
    : latch_(1),
      thread_(std::bind(&EventLoopThread::LoopInThread, this), name),
      loop_(nullptr),
      initCallback_(cb) {}

EventLoopThread::~EventLoopThread() {
    if (loop_ != nullptr) {
        loop_->Quit();
        thread_.Join();
    }
}

EventLoop *EventLoopThread::StartLoop() {
    assert(!thread_.Started());
    thread_.Start();
    latch_.Wait();
    return loop_;
}

void EventLoopThread::LoopInThread() {
    // 子线程创建 EventLoop 并进入loop.
    EventLoop loop;
    if (initCallback_)
        initCallback_();

    loop_ = &loop;
    latch_.CountDown();
    loop.Loop();
}