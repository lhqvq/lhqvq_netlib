/**
 * lhqvq
 * 
 * 
*/

#ifndef __COUNTDOWNLATCH_H__
#define __COUNTDOWNLATCH_H__

#include "src/utils/Uncopyable.h"
#include "src/thread/Mutex.h"
#include "src/thread/Condition.h"

namespace lhqvq {

/**
 * @brief 倒计时门闩
*/
class CountDownLatch : public Uncopyable {
public:
    explicit CountDownLatch(int count);
    ~CountDownLatch() = default;

    /**
     * @brief 递减计数器，减为 0 时会唤醒所有线程
    */
    void CountDown();

    /**
     * @brief 阻塞等待计数器为 0 的条件发生
    */
    void Wait();

    /**
     * @brief 获取计数器
    */
    int GetDown() const;

private:
    mutable MutexLock lock_;
    Condition condition_;
    int count_;
};


} // namespace lhqvq


#endif // __COUNTDOWNLATCH_H__