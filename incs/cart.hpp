#pragma once

#include "../incs/main.hpp"

class Cart
{
    public:
        Cart() {};
        Cart(std::string filename);
        ~Cart();

        std::string filename;
        u8 *rom_data;

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

        static const std::vector<std::string> rom_types;

        static const std::map<int, std::string> lic_codes;

        class CartLoadFailedExeption: public std::exception
        {
            public:
                virtual const char *what() const throw()
                {
                    return ("Error loading cart");
                }
        };

        std::string cart_lic_name() const;
        std::string cart_type_name() const;
        bool cart_load(std::string filename);
        u8 cart_read(u16 address);
        void cart_write(u16 address, u8 value);
};

