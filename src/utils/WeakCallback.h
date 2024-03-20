/**
 * lhqvq
 * 
 * 
*/

#ifndef __WEAKCALLBACK_H__
#define __WEAKCALLBACK_H__

#include <memory>
#include <functional>

namespace lhqvq {

/**
 * @brief 弱回调函数，与仿函数类似，区别在于该类会使用 weak_ptr 判断对象是否存活，若是才调用
*/
template <typename T, typename... Args>
class WeakCallback {
public:
    WeakCallback(const std::weak_ptr<T> &obj_ptr,
                 const std::functional<void(T*, Args...)> &fun)
        : obj_ptr(obj_ptr), fun_(fun) {}
    ~WeakCallback() = default;

    void operator()(Args... args) const {
        std::shared_ptr<T> ptr(obj_ptr_.lock());
        if (ptr) {
            fun_(ptr.get(), std::forward<Args>(args)...);
        }
    }

private:
    std::weak_ptr<T> obj_ptr_;
    std::functional<void(T*, Args...)> fun_;
};

template <typename T, typename... Args>
WeakCallback<T, Args...> make_weakcallback(const std::shared_ptr<T> &obj_ptr,
                                           void (T::*func_ptr)(Args...)) {
    return WeakCallback<T, Args...>(obj_ptr, func_ptr);
}

template <typename T, typename... Args>
WeakCallback<T, Args...> make_weakcallback(const std::shared_ptr<T> &obj_ptr,
                                           void (T::*func_ptr)(Args...) const) {
    return WeakCallback<T, Args...>(obj_ptr, func_ptr);
}

} // namespace lhqvq

#endif // __WEAKCALLBACK_H__