#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

int main()
{
    std::ifstream ifs("test.txt");
    uint8_t *s;
    int i = 0;
    char c = 0;

    std::filesystem::path filePath = "test.txt";
    uint16_t rom_size = std::filesystem::file_size(filePath);

    s = new uint8_t[rom_size];
    while (ifs.get(c))
    {
        s[i] = c;
        i++;
    }

    for(int j = 0; j < rom_size; j++)
        std::cout << s[j];
    std::cout << std::endl;

    

    ifs.close();
    delete[] s;
    return (0);
}