#pragma once

#include "../incs/main.hpp"
#include "../incs/cart.hpp"

u8 bus_read(u16 adress, Cart *cart);
void bus_write(u16 adress, u8 value, Cart *cart);