#include "../incs/main.hpp"

/*checks if we have a argument
checks if the file name is *.gb*/
bool args_parse(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: ./GB-EMU <rom_file>" << std::endl;
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
