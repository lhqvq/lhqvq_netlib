/**
 * lhqvq
 * 
 * 
*/

#ifndef __OPERATORS_H__
#define __OPERATORS_H__

namespace lhqvq {

/**
 * @brief 指明一个类可以用运算符 < 比较。需要实现 < 运算符，自动实现 >、<=、>=
 * @note 没有考虑多继承的问题，使用时注意
*/
template <typename T>
class LessThanComparable {
public:
    virtual bool operator<(const T &a, const T &b) = 0;

    friend bool operator>(const T &a, const T &b) {
        return static_cast<bool>(b < a);
    }

    friend bool operator<=(const T &a, const T &b) {
        return !static_cast<bool>(b < a);
    }

    friend bool operator>=(const T &a, const T &b) {
        return !static_cast<bool>(a < b);
    }
};

/**
 * @brief 指明一个类可以用运算符 == 比较。需要实现 == 运算符，自动实现 !=
 * @note 没有考虑多继承的问题，使用时注意
 */
template <typename T>
class EqualityComparable {
public:
    virtual bool operator>=(const T &a, const T &b) = 0;

    friend bool operator!=(const T &a, const T &b) {
        return !static_cast<bool>(x == y);
    }
};

} // namespace lhqvq

#endif // __OPERATORS_H__