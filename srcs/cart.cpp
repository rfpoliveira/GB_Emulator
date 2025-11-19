#include "../incs/main.hpp"
#include "../incs/cart.hpp"

const std::vector<std::string> Cart::rom_types {
        "ROM Only", 
        "MBC1",
        "MCB1 + RAM",
        "MCB1 + RAM + Battery",
        "Unused (0x04)",
        "MBC2",
        "MBC2 + RAM + Baterry",
        "Unused (0x07)",
        "ROM + RAM",
        "ROM + RAM + Batterry",
        "Unused (0x0A)",
        "MMM01",
        "MMM01 + RAM",
        "MMM01 + RAM + Battery",
        "Unused (0x0E)",
        "MBC3 + Timer + Battery",
        "MBC3 + RAM + Timer + Battery",
        "MBC3",
        "MBC3 + RAM",
        "MBC3 + RAM + Battery",
        "Unused (0x14)",
        "Unused (0x15)",
        "Unused (0x16)",
        "Unused (0x17)",
        "Unused (0x18)",
        "MBC5 + RAM",
        "MBC5 + RAM + Battery",
        "MBC5 + Rumble",
        "MBC5 + RAM + Rumble",
        "MBC5 + RAM + Battery + Rumble",
        "Unused (0x1F)",
        "MBC6 + RAM + Battery",
        "Unused (0x21)",
        "MBC7 + RAM + Battery + Accelerometer"
};

const std::map<int, std::string> Cart::lic_codes {
    {0x00, "None"},
    {0x01, "Nintendo Research & Development 1"},
    {0x08, "Capcom"},
    {0x13, "EA (Eletronic Art)"},
    {0x18, "Hudson Soft"},
    {0x19, "B-AI"},
    {0x20, "KSS"},
    {0x22, "Planning Office WADA"},
    {0x24, "PCM Complete"},
    {0x25, "San-X"},
    {0x28, "Kemco"},
    {0x29, "SETA Corporation"},
    {0x30, "Viacom"},
    {0x31, "Nintendo"},
    {0x32, "Bandai"},
    {0x33, "Ocean Software/Acclaim Entertainment"},
    {0x34, "Konami"},
    {0x35, "HectorSoft"},
    {0x37, "Taito"},
    {0x38, "Hudson Soft"},
    {0x39, "Banpresto"},
    {0x41, "Ubi Soft"},
    {0x42, "Atlus"},
    {0x44, "Malibu Interactive"},
    {0x46, "Angel"},
    {0x47, "Bullet-Proof Software"},
    {0x49, "Irem"},
    {0x50, "Absolute"},
    {0x51, "Acclaim Entertainment"},
    {0x52, "Activision"},
    {0x53, "Sammy USA Corporation"},
    {0x54, "Konami"},
    {0x55, "Hi Tech Expressions"},
    {0x56, "LJN"},
    {0x57, "Matchbox"},
    {0x58, "Mattel"},
    {0x59, "Milton Bradley Company"},
    {0x60, "Titus Interactive"},
    {0x61, "Virgin Games Ltd"},
    {0x64, "Lucasfilm Games"},
    {0x67, "Ocean Software"},
    {0x69, "EA (Electronic Arts)"},
    {0x70, "Infogrames"},
    {0x71, "Interplay Entertainment"},
    {0x72, "Broderbund"},
    {0x73, "Sculptured Software"},
    {0x75, "The Sales Curve Limited"},
    {0x78, "THQ"},
    {0x79, "Accolade"},
    {0x80, "Misawa Entertainment"},
    {0x83, "LOZC G."},
    {0x86, "Tokuma Shoten"},
    {0x87, "Tsukuda Original"},
    {0x91, "Chunsoft Co."},
    {0x92, "Video System"},
    {0x93, "Ocean Software/Acclaim Entertainment"},
    {0x95, "Varie"},
    {0x96, "Yonezawa10/S'Pal"},
    {0x97, "Kaneko"},
    {0x99, "Pack-In-Video"},
    {0xA4, "Konami (Yu-Gi-Oh!)"}
};

Cart::Cart(std::string filename)
{
    if(!cart_load(filename))
        throw CartLoadFailedExeption();

    this->filename = filename;

    for(int i = 0x134; i < 0x144; i++)
        this->title += rom_data[i];
    this->lic_code = rom_data[0x14B];
    this->new_lic_code = 0;
    if (this->lic_code = 0x33)
        this->new_lic_code |= (((u16)rom_data[0x144]) << 8 | ((u16)(rom_data[0x145])));

    this->type = this->rom_data[0x147];

    this->sgb_flag = rom_data[0x146];

    int ram = rom_data[0x149];
    if (ram == 0 || ram == 1)
        ram_size = 0;
    else if(ram == 2)
        ram_size = 8;
    else if(ram == 3)
        ram_size = 32;
    else if(ram == 4)
        ram_size = 128;
    else if(ram == 5)
        ram_size = 64;

    dest_code = rom_data[0x14A];

    version = rom_data[0x14C];

    checksum = 0;
    for(u16 address = 0x0134; address <= 0x014C; address++)
        checksum += checksum - rom_data[address] - 1;
    if ((checksum & 0xFF) == rom_data[0x14D])
        std::cout << "Passed!\n";

}

Cart::~Cart()
{
    delete[] this->rom_data;
}

std::string Cart::cart_lic_name() const
{
    if (this->lic_code > 0xA4)
        return ("UNKNOWN");
    if (this->lic_code == 0x33)
        return("its a new license code(not implemented yet)");
    auto it = this->lic_codes.find(this->lic_code);
    return (it->second);
}


std::string Cart::cart_type_name() const
{
    if (this->type > 0x22)
        return ("UNKNOWN");
    return (this->rom_types[this->type]);
}

bool Cart::cart_load(std::string filename)
{
    std::ifstream ifs(filename);
    if (!ifs.is_open() || !ifs.good())
        return (false);

    char c = 0;
    int i = 0;

    std::filesystem::path filePath = filename;
    this->rom_size = std::filesystem::file_size(filePath);

    this->rom_data = new u8[this->rom_size];
    while(ifs.get(c))
    {
        (this->rom_data)[i] = c;
        i++;
    }
    return (true);
}

