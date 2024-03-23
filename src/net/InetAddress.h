/**
 * lhqvq
 *
 *
 */

#ifndef __INETADDRESS_H__
#define __INETADDRESS_H__

#include <netinet/in.h>
#include <string>

#include "src/utils/Copyable.h"

namespace lhqvq {

// ip 地址. 只支持 ipv4.
// 可拷贝的.
class InetAddress : public Copyable {
public:
    // 生成一个无效的地址对象.
    InetAddress();
    // 用指定的 ip, port, 生成地址. 输入参数 port 是主机序.
    InetAddress(const char* ip, uint16_t port);
    InetAddress(const std::string& ip, uint16_t port)
      : InetAddress(ip.c_str(), port) {}
    // 该函数不指定 ip, 生成的地址用于监听服务器, 监听地址为 0.0.0.0.
    InetAddress(uint16_t port);
    InetAddress(const struct sockaddr_in& addr) : addr_(addr) {}

    ~InetAddress() = default;

    socklen_t Len() const { return static_cast<socklen_t>(sizeof(addr_)); }
    const struct sockaddr* SockAddr() const {
        return reinterpret_cast<const struct sockaddr*>(&addr_);
    }
    struct sockaddr* SockAddr() {
        return reinterpret_cast<struct sockaddr*>(&addr_);
    }

    void SetAddress(const struct sockaddr_in& addr) { addr_ = addr; }

    // 获得 IP 地址的字符串表示.
    // 如果 printPort 为 true, 就一并输出端口号.
    std::string ToString(bool printPort) const;
    uint32_t IpInNetEndian() const { return addr_.sin_addr.s_addr; }
    uint64_t PortInNetEndian() const { return addr_.sin_port; }
    sa_family_t Family() const { return addr_.sin_family; }

    // 从主机名(或点分字符串的IP地址)和服务名(或十进制端口号字符串)获取对应的地址结构.
    // 如果同时指定了 hostname 和 service, 返回的地址结构可用于主动连接(connect).
    // 如果 hostname 为 NULL, resultAddr 返回的地址结构将用于被动连接(accept).
    static bool NameToAddress(const char* hostname,
                            const char* service,
                            InetAddress* resultAddr);

private:
    // TODO(GGGGITFKBJG): 增加 ipv6 的支持.
    struct sockaddr_in addr_;
};

} // namespace lhqvq

#endif // __INETADDRESS_H__