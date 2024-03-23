/**
 * lhqvq
 *
 *
 */

#include "src/reactor/Channel.h"

#include <cassert>
#include "src/reactor/EventLoop.h"
#include "src/logger/Logger.h"

using lhqvq::Channel;

Channel::Channel(EventLoop *loop, int fd)
    : loop_(loop),
      registeredToPoller(false),
      fd_(fd),
      events_(kNoneEvent),
      revents_(0) {}

Channel::~Channel() {
    assert(!registeredToPoller);
}

void Channel::HandleEvent() {
    assert(loop_->IsInLoopThread());
    // https://www.man7.org/linux/man-pages/man2/epoll_ctl.2.html
    if (revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)) { // 可读或者对端正常关闭.
        if (readCallback_)
            readCallback_();
    }
    if (revents_ & EPOLLOUT) { // 可写.
        if (writeCallback_)
            writeCallback_();
    }
    if (revents_ & EPOLLERR) { // fd 出错.
        LOG_ERROR << "fd = " << fd_ << ", EPOLLERR.";
        if (errorCallback_)
            errorCallback_();
    }
    if (revents_ & EPOLLHUP) { // 对端异常关闭.
        LOG_WARN << "fd = " << fd_ << ", EPOLLHUP.";
        if (closeCallback_)
            closeCallback_();
    }
}

void Channel::RegisterToPoller() {
    loop_->UpdateChannel(this);
    registeredToPoller = true; // 已注册.
    LOG_TRACE << "channel " << fd_ << " registered.";
}

void Channel::RemoveFromPoller() {
    assert(events_ == kNoneEvent);
    loop_->RemoveChannel(this);
    registeredToPoller = false; // 取消注册.
    LOG_TRACE << "channel " << fd_ << " unregistered.";
}