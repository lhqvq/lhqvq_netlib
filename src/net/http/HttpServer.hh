/**
 * lhqvq
 *
 *
 */

#ifndef __HTTPSERVER_H__
#define __HTTPSERVER_H__

#include <functional>
#include <string>

#include "src/reactor/EventLoop.h"
#include "src/utils/Uncopyable.h"
#include "src/net/InetAddress.h"
#include "src/net/TcpServer.h"
#include "src/net/http/HttpRequest.h"
#include "src/net/http/HttpResponse.h"

namespace lhqvq {

class HttpServer : public Uncopyable {
public:
  using HttpCallback = std::function<void (const HttpRequest&,
                                           HttpResponse*)>;

  HttpServer(EventLoop* loop,
             const InetAddress& listenAddr,
             const std::string& name,
             bool reusePort = false);

  EventLoop* GetLoop() const { return server_.OwerLoop(); }
  void SetHttpCallback(const HttpCallback& cb) { httpCallback_ = cb; }
  void Start(int numThreads);

private:
  void onConnection(const TcpConnectionPtr& conn);
  void onMessage(const TcpConnectionPtr& conn,
                 Buffer* buf,
                 Timestamp receiveTime);
  void handleRequest(const TcpConnectionPtr& conn, const HttpRequest& req);

  TcpServer server_;
  HttpCallback httpCallback_;
};

} // namespace lhqvq

#endif // __HTTPSERVER_H__