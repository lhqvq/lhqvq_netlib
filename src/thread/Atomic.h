/**
 * lhqvq
 * 
 * 
*/

#ifndef __ATOMIC_H__
#define __ATOMIC_H__

#include <cstdint>

#include "src/utils/Uncopyable.h"

namespace lhqvq {
namespace detail {

/**
 * @brief 原子类型，不可拷贝
 * @note 使用__ATOMIC_SEQ_CST内存模型.
 * https://gcc.gnu.org/onlinedocs/gcc/_005f_005fatomic-Builtins.html
*/
template <typename T>
class AtomicInterger : public Uncopyable {
public:
    AtomicInterger() : value_(0) {}
    explicit AtomicInterger(T value) : value_(value) {}
    ~AtomicInterger() = default;

    T Get() {
        return __atomic_load_n(&value_, __ATOMIC_SEQ_CST);
    }

    T GetAndSet(T newValue) {
        return __atomic_exchange_n(&value_, newValue, __ATOMIC_SEQ_CST);
    }

    T GetAndAdd(T x) {
        return __atomic_fetch_add(&value_, x, __ATOMIC_SEQ_CST);
    }

    T AddAndGet(T x) {
        return __atomic_add_fetch(&value_, x, __ATOMIC_SEQ_CST);
    }

    T IncrementAndGet() {
        return AddAndGet(1);
    }

    T DecrementAndGet() {
        return AddAndGet(-1);
    }
private:
    volatile T value_;
};

} // namespace detail

using AtomicInt32 = detail::AtomicInterger<int32_t>;
using AtomicInt64 = detail::AtomicInterger<int64_t>;

} // namespace lhqvq

#endif // __ATOMIC_H__