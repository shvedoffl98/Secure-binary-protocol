#include "channel_udp.h"


namespace shadow
{

namespace network
{

SocketUDP::SocketUDP(udp_ch_init_dto_t&& cfg) noexcept(true)
    : sock_fd {udp_ch_traits_t::sock_fd_not_inited},
      epfd {udp_ch_traits_t::sock_fd_not_inited},
      config(std::move(cfg)),
      r_mtx {},
      w_mtx {}
{
    if (!_init()) {
        _close_save();
    }
}

SocketUDP::~SocketUDP() noexcept(true)
{
    _close_save();
}

std::optional<std::vector<uint8_t>> SocketUDP::read_impl()
{
    /**
     * TODO: Make expandable to work with 1+ sockets.
     */

    std::lock_guard<std::mutex> lock(r_mtx);

    /* Return value */
    std::optional<std::vector<uint8_t>> ret_val {std::nullopt};

    if (sock_fd != udp_ch_traits_t::sock_fd_not_inited) {
        /* Just one socket fd */
        struct epoll_event events[1] {};
        int32_t socket_ready {};

        std::vector<uint8_t> vec {};
        vec.resize(protocol::PACKET_SIZE_BYTES);
        socket_ready = epoll_wait(epfd, events, 1, -1);

        if (/*1*/socket_ready > 0 && events[0].data.fd == sock_fd) {
            struct sockaddr_in addr {};
            memset(&addr, 0, sizeof(sockaddr_in));
            sock_len_t len = sizeof(addr);
            ssize_t recv_bytes = recvfrom(events[0].data.fd,
                                    vec.data(),
                                    protocol::PACKET_SIZE_BYTES,
                                    0,
                                    (struct sockaddr*)&addr,
                                    &len);

            if (recv_bytes <= 0) {
                ret_val = std::nullopt;
            } else {
                vec.resize(recv_bytes);
                ret_val = std::move(vec);
            }
        }
    }
    return ret_val;
}

bool SocketUDP::write_impl(udp_ch_write_dto_t&& cfg)
{
    std::lock_guard<std::mutex> lock(w_mtx);

    /* Return value */
    bool ret_val {false};

    if (sock_fd != udp_ch_traits_t::sock_fd_not_inited) {
        struct sockaddr_in sock_addr {};
        sock_addr = {.sin_family = udp_ch_traits_t::domain_v4,
                        .sin_port = htons(cfg.port)};

        inet_aton(cfg.send2ip.c_str(), &sock_addr.sin_addr);

        sock_len_t len = sizeof(sock_addr);
        if(sendto(sock_fd, cfg.data.data(), cfg.data.size(), 0, (struct sockaddr*)&sock_addr, len) >= 0) {
            ret_val = true;
        }
    }
    return ret_val;
}

bool SocketUDP::_init()
{
    std::scoped_lock lock(r_mtx, w_mtx);

    /* Return value */
    bool ret_val {true};

    if (sock_fd == udp_ch_traits_t::sock_fd_not_inited) {
        struct sockaddr_in sock_addr {};
        sock_addr = {.sin_family = udp_ch_traits_t::domain_v4,
                     .sin_port = htons(config.port)};

        inet_aton(config.ip.c_str(), &sock_addr.sin_addr);

        sock_fd = socket(sock_addr.sin_family,
                        udp_ch_traits_t::type,
                        udp_ch_traits_t::protocol);

        /* Make socket NON blocking*/
        fcntl(sock_fd, F_SETFL, O_NONBLOCK);

        /* If binding fails, */
        if (!_bind(sock_fd, (struct sockaddr*)&sock_addr, sizeof(sock_addr))) {
            ret_val = false;
        } else {
            if (!_init_epoll(EPOLLIN, EPOLL_CTL_ADD)) {
                ret_val = false;
            }
        }
    }
    return ret_val;
}

uint32_t SocketUDP::_bind(socket_fd_t sock_fd,
                struct sockaddr* sock_addr,
                sock_len_t sock_len)
{
    return bind(sock_fd, sock_addr, sock_len)
        != udp_ch_traits_t::sock_fd_not_inited;
}

bool SocketUDP::_init_epoll(EPOLL_EVENTS event, decltype(EPOLL_CTL_ADD) action)
{
    /**
     * TODO: Modify code to process different actions
     */

    bool ret_val {true};
    if (epfd = epoll_create1(0); epfd < 0) {
        ret_val = false;
    } else {
        epoll_event ev {};
        ev.events = event;
        /* debug assert */
        assert(sock_fd > 0);
        ev.data.fd = sock_fd;
        if (epoll_ctl(epfd, action, sock_fd, &ev) < 0) {
            ret_val = false;
        }
    }
    return ret_val;
}

void SocketUDP::_close_save()
{
    /**
     * TODO: Think about error handling.
     */
    std::scoped_lock lock(r_mtx, w_mtx);
    if (close(sock_fd) < 0) {
        /* log error */
    }

    if (close(epfd) < 0) {
        /* log error */
    }
}

}
}
