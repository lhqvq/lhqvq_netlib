/**
 * lhqvq
 *
 *
 */

#ifndef __BLOCKINGQUEUE_H__
#define __BLOCKINGQUEUE_H__

#include <deque>
#include <utility>

#include "src/utils/Uncopyable.h"

namespace lhqvq {

template <typename T>
class BlockingQueue : public Uncopyable {
public:
    BlockingQueue() {}

    /**
     * @brief 往阻塞队列中插入数据
    */
    void Put(const T& x) {
        std::lock_guard<std::mutex> lck(mtx_);
        queue_.emplace_back(x);
        condition_.notify_one();
    }
    void Put(T &&x) {
        std::lock_guard<std::mutex> lck(mtx_);
        queue_.emplace_back(std::move(x));
        condition_.notify_one();
    }


    /**
     * @brief 从阻塞队列中取出数据
     */
    T Pop() {
        std::lock_guard<std::mutex> lck(mtx_);
        while (queue_.empty()) {
            condition_.wait();
        }
        T x = std::move(queue_.front());
        queue_.pop_front();
        return x;
    }


    /**
     * @brief 返回阻塞队列大小
     */
    size_t Size() {
        std::lock_guard<std::mutex> lck(mtx_);
        return queue_.size();
    }


private:
    std::mutex mtx_;
    std::condition_variable condition_;
    std::deque<T> queue_;
};


} // namespace lhqvq

#endif // __BLOCKINGQUEUE_H__