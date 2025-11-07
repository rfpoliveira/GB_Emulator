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
    {"00", "None"},
    {"01", "Nintendo Research & Development 1"},
    {"08", "Capcom"},
    {"13, EA (Eletronic Art)"},
    {"18", "Hudson Soft"},
    {"19", "B-AI"},
    {"20", "KSS"},
    {"22", "Planning Office WADA"},
    {"24", "PCM Complete"},
    {"25", "San-X"},
    {"28", "Kemco"},
    {"29", "SETA Corporation"},
    {"30", "Viacom"},
    {"31", "Nintendo"},
    {"32", "Bandai"},
    {"33", "Ocean Software/Acclaim Entertainment"},
    {"34", "Konami"},
    {"35", "HectorSoft"},
    {"37", "Taito"},
    {"38", "Hudson Soft"},
    {"39", "Banpresto"},
    {"41", "Ubi Soft"},
    {"42", "Atlus"},
    {"44", "Malibu Interactive"},
    {"46", "Angel"},
    {"47", "Bullet-Proof Software"},
    {"49", "Irem"},
    {"50", "Absolute"},
    {"51", "Acclaim Entertainment"},
    {"52", "Activision"},
    {"53", "Sammy USA Corporation"},
    {"54", "Konami"},
    {"55", "Hi Tech Expressions"},
    {"56", "LJN"},
    {"57", "Matchbox"},
    {"58", "Mattel"},
    {"59", "Milton Bradley Company"},
    {"60", "Titus Interactive"},
    {"61", "Virgin Games Ltd"},
    {"64", "Lucasfilm Games"},
    {"67", "Ocean Software"},
    {"69", "EA (Electronic Arts)"},
    {"70", "Infogrames"},
    {"71", "Interplay Entertainment"},
    {"72", "Broderbund"},
    {"73", "Sculptured Software"},
    {"75", "The Sales Curve Limited"},
    {"78", "THQ"},
    {"79", "Accolade"},
    {"80", "Misawa Entertainment"},
    {"83", "LOZC G."},
    {"86", "Tokuma Shoten"},
    {"87", "Tsukuda Original"},
    {"91", "Chunsoft Co."},
    {"92", "Video System"},
    {"93", "Ocean Software/Acclaim Entertainment"},
    {"95", "Varie"},
    {"96", "Yonezawa10/S’Pal"},
    {"97", "Kaneko"},
    {"99", "Pack-In-Video"},
    {"9H", "Bottom Up"},
    {"A4", "Konami (Yu-Gi-Oh!)"}
};

Cart::Cart(std::string filename)
{
    this->filename = filename;
    this->title = "0";

    cart_load();

    std::stringstream tmp;
    tmp << this->rom_data[0x144];
    tmp << this->rom_data[0x145];
    tmp >> this->new_lic_code;
}

Cart::~Cart(){}

std::string Cart::cart_lic_name() const
{
    if (this->new_lic_code > 0xA4)
        return ("UNKNOWN");
    auto it = this->lic_codes.find(this->lic_code);
    return (it->second);
}


std::string Cart::cart_type_name() const
{
    if (this->new_lic_code > 0x22)
        return ("UNKNOWN");
    return (this->rom_types[this->type]);
}

void Cart::cart_load()
{
    std::ifstream ifs(this->filename);

    while (getline(ifs, this->rom_data));
    ifs.close();

    std::filesystem::path filePath = filename;
    this->rom_size = std::filesystem::file_size(filePath);
}

