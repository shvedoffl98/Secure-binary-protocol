#pragma once

#include <vector>
#include <type_traits>
#include <arpa/inet.h>
#include <climits>
#include <span>

#include "../protocol/protocol_structure.h"
#include "../utils/utils.h"


namespace shadow
{

namespace parser
{

class MessageDeserializer
{
public:
    auto parse(std::vector<uint8_t>&& vec)
    {
    }

    static decltype(shadow::protocol::protocol_header_t::message_type)
        get_msg_type(std::span<uint8_t> view)
    {
        using ret_val_t = std::invoke_result_t<decltype(get_msg_type), std::span<uint8_t>>;
        auto val = view[offsetof(shadow::protocol::protocol_header_t,
                                 shadow::protocol::protocol_header_t::message_type)];
        return val;
    }
};

}
}
