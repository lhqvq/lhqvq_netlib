/**
 * lhqvq
 * 
 * 
*/

#ifndef __CURRENTTHREAD_H__
#define __CURRENTTHREAD_H__

#include <cstdint>

namespace lhqvq {
namespace current_thread {


// 缓存了当前线程id, 线程局部数据.
// 目的是为了尽量减少系统调用次数.
extern __thread int t_tid;

extern __thread const char* t_name;

// 获取当前线程tid.
// 如果没有缓存过, 就调用系统调用将其缓存. 否则返回缓存的值.
int Tid();

const char* Name();

// 如果当前线程id等于主线程id, 返回true.
bool IsMainThread();

// 使当前线程休眠usec微秒的时间.
void SleepUsec(int64_t usec);


} // namespace current_thread
} // namespace lhqvq

#endif // __CURRENTTHREAD_H__