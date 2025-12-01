#include "../incs/main.hpp"
#include "../incs/emu.hpp"
#include "../incs/cpu.hpp"
#include "../incs/cart.hpp"

static Emulator emu;

int main (int argc, char **argv)
{
    run_emu(argc, argv);

    return (close_emu());
}

int run_emu(int argc, char** argv)
{
    if (!args_parse(argc, argv))
        return(-1);


    try
    {
        Data data(argv[1]);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return (-2);
    }
    
    std::cout << "Cartrige loaded..." << std::endl;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL_Init failed" << std::endl;
        return (-3);
    }

    if (TTF_Init() < 0)
    {
        std::cout << "TTF_Init failed" << std::endl;
        return (-4);
    }

    if (cpu_init() < 0)
        return (-5);

    emu.running = 1;

    while (emu.running)
    {
        if (emu.paused)
        {
            SDL_Delay(10);
            continue;
        }
        if (!cpu_step())
        {
            std::cout << "Cpu stopped" << std::endl;
            return (-6);
        }
        emu.ticks++;
        break;
    }

    return (0);
}