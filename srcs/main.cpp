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

int close_emu(void)
{
    SDL_Quit();
    TTF_Quit();

    return (0);
}

int run_emu(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: ./RFPO-GB <rom_file>" << std::endl;
        return (-1);
    }

    Cart *cart = new Cart(argv[1]);

    std::cout << cart->cart_lic_name() << "\n";
    std::cout << cart->cart_type_name() << "\n";

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

    delete cart;
    return (0);
}