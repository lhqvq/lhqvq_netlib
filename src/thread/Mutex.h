/**
 * lhqvq
 * 
 * 
*/

#ifndef __MUTEX_H__
#define __MUTEX_H__

#include <pthread.h>
#include <cassert>

#include "src/thread/CurrentThread.h"
#include "src/utils/Uncopyable.h"

// 检查返回值是否为0.
#define MCHECK(ret) { decltype(ret) errnum = ret; assert(errnum == 0); }

namespace lhqvq {

/**
 * @brief Unix mutex 的封装类，不可拷贝
*/
class MutexLock : public Uncopyable {
public:
    MutexLock() : holder_(0) {
        MCHECK(pthread_mutex_init(&mutex_, nullptr));
    }

    ~MutexLock() {
        assert(holder_ == 0);
        MCHECK(pthread_mutex_destroy(&mutex_));
    }

    void Lock() {
        MCHECK(pthread_mutex_lock(&mutex_));
        AssignHolder();
    }

    void Unlock() {
        UnassignHolder();
        MCHECK(pthread_mutex_unlock(&mutex_));
    }

    bool LockedByThisThread() { return holder_ == current_thread::Tid(); }
    pthread_mutex_t* GetPthreadMutex() { return &mutex_; }

    void AssignHolder() { holder_ = current_thread::Tid(); }
    void UnassignHolder() { holder_ = 0; }

private:
    pthread_mutex_t mutex_;
    pid_t holder_;  // 锁持有者的线程id.
};

/**
 * @brief RAII 包装类，不可考呗
*/
class MutexLockGuard : public Uncopyable {
public:
    explicit MutexLockGuard(MutexLock& lock) : lock_(lock) { lock_.Lock(); }
    ~MutexLockGuard() { lock_.Unlock(); }

private:
    MutexLock& lock_;
};


} // namespace lhqvq

#endif // __MUTEX_H__