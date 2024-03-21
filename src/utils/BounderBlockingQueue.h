/**
 * lhqvq
 *
 *
 */

#ifndef __BOUNDERBLOCKINGQUEUE_H__
#define __BOUNDERBLOCKINGQUEUE_H__

#include <functional>
#include <mutex>
#include <condition_variable>
#include <deque>

#include "src/utils/Uncopyable.h"

namespace lhqvq {

template <typename T>
class BounderBlockingQueue : public Uncopyable {
public:
    explicit BounderBlockingQueue(uint32_t maxSize) : capacity_(maxSize) {}

    /**
     * @brief 往阻塞队列中插入数据
    */
    void Put(const T& x) {
        std::lock_guard<std::mutex> lck(mtx_);
        while (queue_.size() == capacity_) {
            not_full_.wait();
        }
        assert(queue_.size() != capacity_);
        queue_.emplace_back(x);
        not_empty_.notify_one();
    }
    void Put(T &&x) {
        std::lock_guard<std::mutex> lck(mtx_);
        while (queue_.size() == capacity_) {
            not_full_.wait();
        }
        assert(queue_.size() != capacity_);
        queue_.emplace_back(std::move(x));
        not_empty_.notify_one();
    }


    /**
     * @brief 从阻塞队列中取出数据
     */
    T Pop() {
        std::lock_guard<std::mutex> lck(mtx_);
        while (queue_.empty()) {
            not_empty_.wait();
        }
        T x = std::move(queue_.front());
        queue_.pop_front();
        not_full_.notify_one();
        return x;
    }


    /**
     * @brief 返回阻塞队列大小
     */
    size_t Size() {
        std::lock_guard<std::mutex> lck(mtx_);
        return queue_.size();
    }

    
    bool Empty() {
        std::lock_guard<std::mutex> lck(mtx_);
        return queue_.empty();
    }


    bool Full() {
        std::lock_guard<std::mutex> lck(mtx_);
        return queue_.size() == capacity_;
    }

private:
    std::mutex mtx_;
    std::condition_variable not_empty_;
    std::condition_variable not_full_;
    std::deque<T> queue_;
    uint32_t capacity_;
};


} // namespace lhqvq

#endif // __BOUNDERBLOCKINGQUEUE_H__