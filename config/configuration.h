#pragma once

#include <cstdint>


namespace secmes
{

namespace config
{

class Configuration
{
public:
    enum class side_type_e : uint8_t
    {
        CLIENT_SIDE = 0,
        SERVER_SIDE
    };
};
}
}
