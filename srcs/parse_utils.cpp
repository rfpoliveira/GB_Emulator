#include "../incs/main.hpp"

bool args_parse(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: ./RFPO-GB <rom_file>" << std::endl;
        return (false);
    }

    std::string filename = argv[1];
    if (filename.size() <= 3)
    {
        std::cout << "Invalid file name" << std::endl;
        return(false);
    }

    std::string term = filename.substr(filename.size() - 3, filename.size());
    if (term != ".gb")
    {
        std::cout << "Invalid file name" << std::endl;
        return(false);
    }

    return(true);
};
