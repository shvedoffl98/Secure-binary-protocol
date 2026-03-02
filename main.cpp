#include "config/configuration.h"
#include "net/socket_base.h"
#include "protocol/protocol.h"

int main(int argc, char** argv)
{
    secmes::network::SocketUDP udp;
    udp.bind();

    secmes::network::SocketBase<secmes::network::SocketUDP> base;
    base.bind();
    return 0;
}