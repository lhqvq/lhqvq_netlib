/**
 * lhqvq
 * 
 * 
*/

#ifndef __TIMESTAMP_H__
#define __TIMESTAMP_H__

#include <ctime>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <string>

#include "src/utils/Copyable.h"
#include "src/utils/operators.h"

namespace lhqvq {


class Timestamp : public Copyable,
                  public LessThanComparable<Timestamp>,
                  public EqualityComparable<Timestamp> {

public:
    Timestamp() : microsecondsSinceEpoch_(0) {}
    explicit Timestamp(uint64_t microsecondsSinceEpoch) : microsecondsSinceEpoch_(microsecondsSinceEpoch) {}

    bool operator<(const Timestamp &that) {
        return microsecondsSinceEpoch_ < that.microsecondsSinceEpoch_;
    }
    bool operator==(const Timestamp &that) {
        return microsecondsSinceEpoch_ == that.microsecondsSinceEpoch_;
    }
    Timestamp operator+(const Timestamp &that) {
        return Timestamp{microsecondsSinceEpoch_ + that.microsecondsSinceEpoch_};
    }

    /**
     * @return 返回当前时间戳所对应的秒数
     */
    operator double() {
        return static_cast<double>(microsecondsSinceEpoch_ / kMicrosecondsPerSecond);
    }

    /**
     * @return 转换成 timespec 结构
     */
    operator timespec() {
        struct timespec ts;
        ::memset(&ts, 0, sizeof ts);
        ts.tv_sec = static_cast<decltype(ts.tv_sec)>(
            microsecondsSinceEpoch_ / Timestamp::kMicrosecondsPerSecond);
        ts.tv_nsec = static_cast<decltype(ts.tv_nsec)>(
            microsecondsSinceEpoch_ % Timestamp::kMicrosecondsPerSecond *
            Timestamp::kNanosecondsPerMicrosecond);
        return ts;
    }

    /**
     * @return 返回两个时间戳相减，用户需保证结果非负
     */
    Timestamp operator-(const Timestamp &that){
        assert(microsecondsSinceEpoch_ >= that.microsecondsSinceEpoch_);
        return Timestamp{microsecondsSinceEpoch_ - that.microsecondsSinceEpoch_};
    }

    /**
     * @return 返回自 1970 年 1 月 1 日 00:00 时刻至当前时刻的微秒数
    */
    uint64_t GetMicroseconds() const { return microsecondsSinceEpoch_; }

    /**
     * @return 获取时间戳对应的日历时间（秒）
    */
    time_t GetSeconds() const {
        return static_cast<time_t>(microsecondsSinceEpoch_ / kMicrosecondsPerSecond);
    }

    /**
     * 将时间戳按照天数向下取整
     * @return 返回本地时间当天 00:00 时刻的时间戳
    */
    Timestamp GetDay() const;

    /**
     * 格式化字符串，yyyyMMdd-HHmmss
     * @param microseconds: 是否精确到微妙
     * @return 返回格式化字符串 yyyyMMdd-HHmmss
     */
    std::string ToString(bool microseconds = false) const;

    /**
     * @return 返回当前时刻时间戳
     */
    static Timestamp Now();
    static Timestamp Invalid() { return Timestamp{}; }

    void swap(Timestamp &that) {
        std::swap(microsecondsSinceEpoch_, that.microsecondsSinceEpoch_);
    }

    static constexpr uint32_t kMicrosecondsPerSecond = 1000000;
    static constexpr uint32_t kNanosecondsPerMicrosecond = 1000;

private:
    // 自 1970 年 1 月 1 日 00:00 时刻至当前时刻的微秒数
    uint64_t microsecondsSinceEpoch_;
};

inline bool operator<(const Timestamp &lhs, const Timestamp &rhs) {
    return lhs.GetMicroseconds() < rhs.GetMicroseconds();
}

inline bool operator==(const Timestamp &lhs, const Timestamp &rhs) {
    return lhs.GetMicroseconds() == rhs.GetMicroseconds();
}

} // namespace lhqvq

#endif // __TIMESTAMP_H__