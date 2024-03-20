/**
 * lhqvq
 * 
 * 
*/

#ifndef __UNCOPYABLE_H__
#define __UNCOPYABLE_H__

namespace lhqvq {

/**
 * @brief 标记一个类是不可拷贝构造的（对象语义），需要时继承它
*/
class Uncopyable {
protected:
    Uncopyable() = default;
    Uncopyable(const Uncopyable &) = default;
    Uncopyable& operator=(const Uncopyable &) = default;
    ~Uncopyable() = default;
};


} // namespace lhqvq

#endif // __UNCOPYABLE_H__