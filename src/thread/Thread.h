/**
 * lhqvq
 * 
 * 
*/

#ifndef __THREAD_H__
#define __THREAD_H__

#include <functional>
#include <string>

#include "src/utils/Uncopyable.h"
#include "src/thread/Atomic.h"
#include "src/thread/CountDownLatch.h"

namespace lhqvq {


/**
 * @brief pthread线程的封装类. 不可拷贝.
*/
class Thread : public Uncopyable {
public:
    using ThreadFunc = std::function<void()>;

    explicit Thread(ThreadFunc func, const std::string &name);
    // 如果正在运行且没有join, 就将线程分离.
    ~Thread();

    // 线程开始执行. 在这里真正地创建了pthread线程.
    void Start();
    void Join();
    bool Started() const { return started_; }
    bool Joined() const { return joined_; }
    pid_t Tid() const { return tid_; }
    pthread_t PhtreadId() const { return pthreadId_; }
    std::string Name() const { return name_; }
    static int NumCreated() { return numCreated_.Get(); }

private:
    ThreadFunc func_;
    std::string name_;
    bool started_;
    bool joined_;
    pid_t tid_;
    pthread_t pthreadId_;
    CountDownLatch latch_;          // 用于线程创建时与子线程间的同步.
    static AtomicInt32 numCreated_; // 已创建线程数.
};


} // namespace lhqvq

#endif // __THREAD_H__