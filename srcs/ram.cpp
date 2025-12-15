#include "../incs/ram.hpp"

u8 RAM::wram_read(u16 address)
{
    address -= 0xC000; //subtracting the starting position to get it to strart from 0x0;

    if (address >= 0x2000)
    {
        std::cerr << "Error on wram_read\n";
        throw RAMaddressOOBExeption();
    }
    return (wram[address]);
}
void RAM::wram_write(u16 address, u8 value)
{
    address -= 0xC000; //subtracting the starting position to get it to strart from 0x0;

    wram[address] = value;
}

u8 RAM::hram_read(u16 address)
{
    address -= 0xFF80; //subtracting the starting position to get it to strart from 0x0;

    return (hram[address]);
}
void RAM::hram_write(u16 address, u8 value)
{
    address -= 0xFF80; //subtracting the starting position to get it to strart from 0x0;

    hram[address] = value;
}