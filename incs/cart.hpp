#pragma once

#include "../incs/main.hpp"

class Cart
{
    public:
        Cart();
        ~Cart();

        u8 entry[4];
        u8 logo[0x30];

        std::string title;
        u16 new_lic_code;
        u8 sgb_flag;
        u8 type;
        u8 rom_size;
        u8 ram_size;
        u8 dest_code;
        u8 lic_code;
        u8 version;
        u8 checksum;
        u16 global_checksum;

        int cart_load(std::string rom);
};

