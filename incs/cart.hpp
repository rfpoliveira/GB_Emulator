#pragma once

#include "../incs/main.hpp"

class Cart
{
    public:
        Cart(std::string filename);
        ~Cart();

        std::string filename;
        std::string rom_data;

        u8 entry[4];
        u8 logo[0x30];

        std::string title;
        u16 new_lic_code;
        u8 sgb_flag;
        u8 type;
        u32 rom_size;
        u8 ram_size;
        u8 dest_code;
        u8 lic_code;
        u8 version;
        u8 checksum;
        u16 global_checksum;

        static const std::vector<std::string> rom_types;

        static const std::map<int, std::string> lic_codes;

        class CartLoadFailedExeption: std::exception
        {
            public:
                virtual const char *what() const throw()
                {
                    return ("Error loading cart");
                }
        };

        std::string cart_lic_name() const;
        std::string Cart::cart_type_name() const;
        void cart_load();
};

