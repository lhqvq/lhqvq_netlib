/**
 * lhqvq
 *
 *
 */

#ifndef __ASYNCLOGGING_H__
#define __ASYNCLOGGING_H__

#include <atomic>
#include <cstring>
#include <memory>
#include <vector>
#include <string>

#include "src/thread/Condition.h"
#include "src/thread/CountDownLatch.h"
#include "src/thread/Mutex.h"
#include "src/thread/Thread.h"
#include "src/utils/Uncopyable.h"

namespace lhqvq {
namespace detail {

constexpr size_t kSmallBufferSize = 4 * 1024;  // 4KiB
constexpr size_t kLargeBufferSize = 4 * 1024 * 1024;  // 4MiB

template <size_t SIZE>
class FixedBuffer : public Uncopyable {
public:
    FixedBuffer() : cur_(buffer_) {}
    ~FixedBuffer() = default;

    void MemZero() { ::memset(buffer_, 0, sizeof(buffer_)); }
    // 向缓冲区内追加 len 字节.
    // 只有在空间足够的情况下会成功.
    void Append(const char* data, size_t len) {
        if (AvailableBytes() > len) {
        ::memcpy(cur_, data, len);
        cur_ += len;
        }
    }
    size_t AvailableBytes() const {
        return sizeof(buffer_) - static_cast<size_t>(cur_ - buffer_);
    }
    size_t WrittenBytes() const {
        return static_cast<size_t>(cur_ - buffer_);
    }
    void Reset() { cur_ = buffer_; }
    const char* Data() const { return buffer_; }

private:
    char buffer_[SIZE];
    char* cur_;
};
} // namespace detail

// 异步的日志系统后端组件.
// 使用者应保证该类对象是全局唯一的.
// 如果使用了该类, 最好不要手动调用Logger::SetOutputFunc.
class AsyncLogging : public Uncopyable {
public:
    AsyncLogging(const std::string &basename,
                 size_t rollsize,
                 int flushInterval = 3);
    ~AsyncLogging();
    // 将日志放入缓冲区队列, 等待后台线程写入文件.
    // 该函数由日志系统前端调用.
    // 是线程安全的.
    void Append(const char *line, size_t len);
    // 启动异步日志系统. 会创建一个后台线程做文件写入的工作.
    // 注意: 必须在子线程开启之前由主线程执行.
    void Start();
    // 关闭异步日志系统.
    // 注意: 必须在所有子线程都退出后由主线程执行.
    void Stop();

private:
    using Buffer = detail::FixedBuffer<detail::kLargeBufferSize>;
    using BufferVector = std::vector<std::unique_ptr<Buffer>>;
    using BufferPtr = BufferVector::value_type;

    static AsyncLogging *asyncLog; // 指向全局唯一对象.
    // 前端指定的输出函数.
    static void AsyncOutput(const char *line, size_t len);
    // 后台线程反复地从缓冲队列中取出并写入文件.
    void ThreadFunc();

    std::string basename_;
    off_t rollsize_;
    int flushInterval_;
    std::atomic<bool> running_;

    Thread thread_; // 后台工作线程.
    MutexLock lock_;
    Condition notEmpty_;
    CountDownLatch latch_;

    BufferVector buffers_;    // 缓冲区队列.
    BufferPtr currentBuffer_; // 当前正使用的缓冲区.
    BufferPtr nextBuffer_;    // 备用缓冲区.
};

} // namespace lhqvq

#endif // __ASYNCLOGGING_H__