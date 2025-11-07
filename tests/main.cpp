#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

int main()
{
    std::ifstream ifs("test.txt");
    std::string s;

    while (getline(ifs, s))
        std::cout << s << "\n";
    ifs.close();

    std::filesystem::path filePath = "./test.txt";
    std::cout << "file is " << std::filesystem::file_size(filePath) << " bytes\n";
    return (0);
}