#pragma once

#include "../incs/main.hpp"
#include "../incs/cart.hpp"
#include "../incs/ram.hpp"

u8 bus_read(u16 address, Cart *cart, RAM *ram);
void bus_write(u16 address, u8 value, Cart *cart, RAM *ram);

u16 bus_read_16bit(u16 address, Cart *cart, RAM *ram);
void bus_write_16bit(u16 address, u16 value, Cart *cart, RAM *ram);