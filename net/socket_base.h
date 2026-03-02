#pragma once

#include <concepts>
#include <type_traits>
#include <iostream>

namespace secmes
{

namespace network
{

template <typename T>
concept SocketLike = requires(T t) {
    { t.bind_impl() } -> std::same_as<void>;
    { t.init_impl() } -> std::same_as<void>;
    { t.read_impl() } -> std::same_as<void>;
    { t.write_impl() } -> std::same_as<void>;
    { t.close_impl() } -> std::same_as<void>;
};

template <typename SocketDerived>
class SocketBase
{
public:
    SocketBase() = default;
    ~SocketBase() = default;

public:
    void bind() requires SocketLike<SocketDerived> { static_cast<SocketDerived*>(this)->bind_impl(); }
    void init() requires SocketLike<SocketDerived> { static_cast<SocketDerived*>(this)->init_impl(); }
    void read() requires SocketLike<SocketDerived> { static_cast<SocketDerived*>(this)->read_impl(); }
    void write() requires SocketLike<SocketDerived> { static_cast<SocketDerived*>(this)->write_impl(); }
    void close() requires SocketLike<SocketDerived> { static_cast<SocketDerived*>(this)->close_impl(); }
};

class SocketUDP : public SocketBase<SocketUDP>
{
public:
    void bind_impl() {}
    void init_impl() {}
    void read_impl() {}
    void write_impl() {}
    void close_impl() {}
};


class SocketTCP : public SocketBase<SocketTCP>
{
public:
    void bind_impl() {}
    void init_impl() {}
    void read_impl() {}
    void write_impl() {}
    void close_impl() {}
};


class SocketUnix : public SocketBase<SocketUnix>
{
public:
    void bind_impl() {}
    void init_impl() {}
    void read_impl() {}
    void write_impl() {}
    void close_impl() {}
};


class SocketDPDK : public SocketBase<SocketDPDK>
{
public:
    void bind_impl() {}
    void init_impl() {}
    void read_impl() {}
    void write_impl() {}
    void close_impl() {}
};

}
}
