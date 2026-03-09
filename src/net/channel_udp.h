#pragma once

#include <cstring>
#include <utility>
#include <iostream>
#include <mutex>
#include <cstddef>

#include <fcntl.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cassert>

#include "channel_base.h"


namespace shadow
{

namespace network
{


class SocketUDP : public ChannelBase<SocketUDP>
{
/**
 * TODO: MAKE PROPER AND CLEAR ERROR HANDLING
 * EX: - sockets not inited - error
 *     - no data - warning/error
 *     - epoll errors
 *     - ...
 */

public:
    /* UDP config */
    struct udp_ch_init_dto_t
    {
        ip_from_config_t ip {};
        port_type_t port {};
    };

    struct udp_ch_traits_t
    {
        using domain_t = int32_t;
        using protocol_t = int32_t;
        using type_t = int32_t;
        static constexpr domain_t domain_v4 = AF_INET;
        static constexpr domain_t domain_v6 = AF_INET6;
        static constexpr protocol_t protocol = IPPROTO_UDP;
        static constexpr type_t type = SOCK_DGRAM;
        static constexpr int8_t sock_fd_not_inited = -1;
    };

    struct udp_ch_write_dto_t
    {
        ip_from_config_t send2ip {};
        port_type_t port {};
        std::vector<uint8_t> data {};
    };

/* Aliases */
private:
    using sock_len_t = uint32_t;
    using epoll_fd_t = int32_t;

/* Constructors */
public:
    SocketUDP() = delete;
    explicit SocketUDP(udp_ch_init_dto_t&& cfg) noexcept(true);

/* Destructor */
    ~SocketUDP() noexcept(true);

/* Public API */
public:
    std::optional<std::vector<uint8_t>> read_impl();
    bool write_impl(udp_ch_write_dto_t&& cfg);

/* Private methods */
private:
    bool _init();
    uint32_t _bind(socket_fd_t sock_fd, struct sockaddr* sock_addr, sock_len_t sock_len);
    bool _init_epoll(EPOLL_EVENTS event, decltype(EPOLL_CTL_ADD) action);
    void _close_save();

/* Owning objects*/
private:
    socket_fd_t sock_fd;
    epoll_fd_t epfd;

    udp_ch_init_dto_t config;

    std::mutex r_mtx;
    std::mutex w_mtx;
};

}
}
