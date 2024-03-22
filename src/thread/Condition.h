/**
 * lhqvq
 * 
 * 
*/

#ifndef __CONDITION_H__
#define __CONDITION_H__

#include <pthread.h>
#include <cstdint>

#include "src/thread/Mutex.h"
#include "src/utils/Uncopyable.h"

// 检查返回值是否为0.
#define MCHECK(ret) { int errnum = ret; assert(errnum == 0); }

namespace lhqvq {

/**
 * @brief Unix 条件变量的包装类，不可拷贝
*/
class Condition : public Uncopyable {
public:
    explicit Condition(MutexLock &lock) : lock_(lock) {
        MCHECK(pthread_cond_init(&condition_, nullptr));
    }
    ~Condition() {
        MCHECK(pthread_cond_destroy(&condition_));
    }

    /**
     * @brief 阻塞等待某个条件
    */
    void Wait() {
        assert(lock_.LockedByThisThread());
        // 由于pthread_cond_wait会在内部解锁, 所以需要手动修改持有者
        lock_.UnassignHolder();
        MCHECK(pthread_cond_wait(&condition_, lock_.GetPthreadMutex()));
        lock_.AssignHolder();
    }

    /**
     * 
     * @brief 等待 seconds, 如果到期时条件还没有出现就返回true.
    */
    bool WaitForSeconds(double seconds);

    /**
     * @brief 至少唤醒一个线程
    */
    void Notify() {
        MCHECK(pthread_cond_signal(&condition_));
    }

    /**
     * @brief 唤醒所有线程
    */
    void NotifyAll() {
        MCHECK(pthread_cond_broadcast(&condition_));
    }

private:
    MutexLock &lock_;
    pthread_cond_t condition_;
};


} // namespace lhqvq

#undef MCHECK
#endif // __CONDITION_H__