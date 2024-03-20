/**
 * lhqvq
 * 
 * 
*/

#ifndef __COPYABLE_H__
#define __COPYABLE_H__

namespace lhqvq {

/**
 * @brief 标记一个类是可拷贝构造的（值语义），需要时继承它
*/
class Copyable {
protected:
    Copyable() = default;
    ~Copyable() = default;
};


} // namespace lhqvq

#endif // __COPYABLE_H__