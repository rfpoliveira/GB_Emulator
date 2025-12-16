#include "../incs/bus.hpp"
#include "../incs/cpu.hpp"

//memory map:

// 0x0000 - 0x3FFF : ROM Bank 0
// 0x4000 - 0x7FFF : ROM Bank 1 - Switchable

//this is ROM data

// 0x8000 - 0x97FF : CHR RAM
// 0x9800 - 0x9BFF : BG Map 1
// 0x9C00 - 0x9FFF : BG Map 2
// 0xA000 - 0xBFFF : Cartridge RAM
// 0xC000 - 0xCFFF : RAM Bank 0
// 0xD000 - 0xDFFF : RAM Bank 1-7 - switchable - Color only
// 0xE000 - 0xFDFF : Reserved - Echo RAM
// 0xFE00 - 0xFE9F : Object Attribute Memory
// 0xFEA0 - 0xFEFF : Reserved - Unusable
// 0xFF00 - 0xFF7F : I/O Registers
// 0xFF80 - 0xFFFE : Zero Page or High RAM


/*read the passed address from the rom data*/
u8 bus_read(u16 address, Cart *cart, RAM *ram)
{
    if (address < 0x8000) //rom data
        return (cart->cart_read(address));
    else if(address < 0xA000) //char/map data
    {
        //TODO
        std::cerr << "Not implemented yet address < 0xA000(char/maps)" << std::endl;
        return (0xFD);
    }
    else if (address < 0xC000) //cart RAM
        return (cart->cart_read(address));
    else if (address < 0xE000) //WRAM or working ram
        return (ram->wram_read(address));
    else if (address < 0xFE00) //reserved echo ram, we dont need it
        return (0);
    else if (address < 0xFEA0) //object atributed memory (OAM)
    {
        //TODO
        std::cerr << "Not implemented yet address < 0xFEA0(OAM)" << std::endl;
        return (0xFD);
    }
    else if (address < 0xFF00) //unusable
        return (0);
    else if (address < 0xFF80) // i/o registers
    {
        //TODO
        std::cerr << "Not implemented yet address < 0xFF80(i/o)" << std::endl;
        return (0xFD);
    }
    else if (address == 0xFFFF) //CPU enable register
        return (CPU::get_ie_register());

    return (ram->hram_read(address)); //anything bettewn 0xFFFF and 0xFF80, high ram
}


/*write something in the rom data*/
void bus_write(u16 address, u8 value, Cart *cart, RAM *ram)
{
    if (address < 0x8000)
        cart->cart_write(address, value);
        else if(address < 0xA000) //char/map data
        //TODO
        std::cerr << "Not implemented yet address < 0xA000(char/maps)" << std::endl;
    else if (address < 0xC000) //cart RAM
        cart->cart_write(address, value);
    else if (address < 0xE000) //WRAM or working ram
        ram->wram_write(address, value);
    else if (address < 0xFE00) //reserved echo ram, we dont need it
        return ;
    else if (address < 0xFEA0) //object atributed memory (OAM)
        //TODO
        std::cerr << "Not implemented yet address < 0xFEA0(OAM)" << std::endl;
    else if (address < 0xFF00) //unusable
        return ;
    else if (address < 0xFF80) // i/o registers
        //TODO
        std::cerr << "Not implemented yet address < 0xFF80(i/o)" << std::endl;
    else if (address == 0xFFFF) //CPU enable register
        CPU::set_ie_register(value);
    else
        ram->hram_write(address, value); //anything bettewn 0xFFFF and 0xFF80, high ram
}

u16 bus_read_16bit(u16 address, Cart *cart, RAM *ram)
{
    u16 low = bus_read(address, cart, ram);
    u16 high = bus_read(address + 1, cart, ram);    

    return (low | (high << 8));
}

void bus_write_16bit(u16 address, u16 value, Cart *cart, RAM *ram)
{
    bus_write(address + 1, (value >> 8) & 0xFF, cart, ram);
    bus_write(address, (value & 0xFF), cart, ram);
}