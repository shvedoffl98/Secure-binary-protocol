#include "configuration/configuration.h"
#include "net/channel_base.h"
#include "net/channel_udp.h"
#include "protocol/protocol_structure.h"
#include "protocol/tlv.h"
#include "crypto/openssl_wrapper.h"


int main(int argc, char** argv)
{
    shadow::network::SocketUDP::udp_ch_init_dto_t udp_config {.ip = "127.0.0.1", .port = 1112};
    shadow::network::SocketUDP sock_udp(std::move(udp_config));


    // /* SERVER */
    // while(1) {
    //     auto ret = sock_udp.read_base();
    //     if (ret.has_value()) {
    //         for (const auto& b : *ret) {
    //             // Cast std::byte to char for character output
    //             std::cout << static_cast<char>(std::to_integer<int>(b));
    //         }
    //         std::cout << std::endl;
    //     } else {
    //         std::cout << "no value\n";
    //     }
    // }

    // auto [pub, priv] = CryptoProvider::get_pub_priv_keys();

    // for(const auto& x : pub) {
    //     std::cout << x << std::endl;
    // }

    /* CLIENT */
    // std::cout << alignof(shadow::protocol::message_payload_t<shadow::protocol::message_type_e::DATA_UNIT>) << std::endl;
    // std::cout << sizeof(shadow::protocol::message_payload_t<shadow::protocol::message_type_e::DATA_UNIT>) << std::endl;

    std::string str = "Hello, world!";
    std::vector<std::byte> bytes(reinterpret_cast<const std::byte*>(str.data()),
                               reinterpret_cast<const std::byte*>(str.data() + str.size()));
    shadow::network::SocketUDP::udp_ch_write_dto_t cfg {.send2ip = "127.0.0.1",.port=1112, .data=std::move(bytes)};
    sock_udp.write_base(std::move(cfg));
    return 0;
}
