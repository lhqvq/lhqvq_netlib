/**
 * lhqvq
 *
 *
 */

#ifndef __CONNECTOR_H__
#define __CONNECTOR_H__

#include <atomic>
#include <functional>
#include <memory>

#include "src/net/InetAddress.h"
#include "src/net/Socket.h"
#include "src/reactor/Channel.h"
#include "src/reactor/EventLoop.h"
#include "src/reactor/Timer.h"
#include "src/utils/Uncopyable.h"

namespace lhqvq {

// 连接请求发起器.
// 连接成功后会调用回调 NewConnectionCallback, 转移已连接套接字的管理权.
class Connector : public Uncopyable,
                  public std::enable_shared_from_this<Connector> {
public:
    using NewConnectionCallback =
        std::function<void (SocketPtr connSocket,
                        const InetAddress& serverAddr)>;

    Connector(EventLoop* loop, const InetAddress& serverAddr);
    ~Connector();

    void SetNewConnectionCallback(const NewConnectionCallback& cb) {
        newConnectionCallback_ = cb;
    }
    InetAddress ServerAddress() const { return serverAddr_; }

    // 尝试连接, 如果失败会按一定间隔重试, 直到连接成功或者调用 Cancel().
    // 只能调用一次, 如果需要重启请使用 Restart().
    void Start();
    // 可以多次调用, 新调用会关闭旧的套接字(及其连接), 换用新的套接字.
    // 会重新初始化内部的状态参数.
    // 只能在 loop 线程中执行.
    void Restart();
    // 取消连接.
    void Stop();

private:
    void StartInLoop();
    void StopInLoop();
    // 不会阻塞. 在 poller 中注册对套接字可写事件的关注.
    // 用来处理 connect 返回出错但又不能重启的情况.
    void RegisterForConnecting();
    void UnregisterForConnecting();
    // 处理套接字可写(等待的连接已完成)事件.
    void HandleWrite();
    // connect 暂时失败后间隔重试.
    void Retry();

    EventLoop* loop_;
    SocketPtr socket_;  // 主动连接套接字.
    std::unique_ptr<Channel> socketChannel_;  // 与主动连接套接字关联.
    const InetAddress serverAddr_;
    bool connecting_;
    int retryDelay_;  // 首次重试间隔时间(秒).
    std::atomic<bool> stop_;
    TimerIndex timerIdx_;  // 用于取消重试的计时器索引.
    NewConnectionCallback newConnectionCallback_;

    static constexpr int kInitRetryDelay = 1;  // 首次重启间隔时间.
    static constexpr int kMaxRetryDelay = 30;  // 最大重试间隔时间.
};

using ConnectorPtr = std::shared_ptr<Connector>;

} // namespace lhqvq

#endif // __CONNECTOR_H__