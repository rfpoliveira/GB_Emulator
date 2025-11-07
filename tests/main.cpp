#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

int main()
{
    std::ifstream ifs("test.txt");
    std::string s;
    std::string save;

    while (getline(ifs, s))
    {
        std::cout << s << "\n";
        save += s;
    }

    std::cout << "testing: " << s[0x1] << std::endl;

    ifs.close();
    return (0);
}