#pragma once
#include <cstdint>

struct client_bits_t
{
    int array [ 7 ];
};

struct name_entry
{
    uint32_t index;
    char name [ 36 ];
    uint8_t pad [ 92 ];
    int32_t health;
    uint8_t pad2 [ 70 ];
};