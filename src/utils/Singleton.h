/**
 * lhqvq
 *
 *
 */

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <pthread.h>
#include <cassert>
#include <cstdlib>

#include "src/utils/Uncopyable.h"

namespace lhqvq
{

/**
 * @brief 单例模板，模板形参必须有默认构造函数
*/
template <typename T>
class Singleton : public Uncopyable {
public:
    Singleton() = delete;
    ~Singleton() = delete;

    /**
     * @brief C++ 11 要求编译器对这段代码做多线程安全的优化
    */
    static T& GetInstance() {
        ptr_ = new T();
        return *ptr_;
    }

private:
    static T *ptr_;
};

template <typename T>
T *Singleton<T>::ptr_ = nullptr;


} // namespace lhqvq

#endif // __SINGLETON_H__