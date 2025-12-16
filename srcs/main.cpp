#include "../incs/main.hpp"
#include "../incs/bus.hpp"
#include "../incs/cart.hpp"
#include "../incs/emu.hpp"
#include "../incs/instructions.hpp"
#include "../incs/cpu.hpp"

Emulator emu;

int main (int argc, char **argv)
{
    run_emu(argc, argv);

    return (close_emu());
}

/*when we return we must free the memery from cart and cpu*/
int r_delete(Cart *cart, CPU *cpu, RAM *ram, int code)
{
    if (cart)
        delete cart;
    if (cpu)
        delete cpu;
    if (ram)
        delete ram;
    return(code);
}


/*main emualtor loop, we do some basic parsing(is the file is valid, *.gb)
then create cart and cpu classes
iniciate the visual library variables and the cpu
and enter the cpu loop*/
int run_emu(int argc, char** argv)
{
    if (!args_parse(argc, argv))
        return(-1);

    RAM *ram = nullptr;
    Cart *cart = nullptr;
    CPU *cpu = nullptr;
    try
    {
        ram = new RAM();
        cart = new Cart(argv[1]);
        cpu = new CPU(cart, ram);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return(r_delete(cart, cpu, ram, -2));
    }
    std::cout << "Cartrige loaded..." << std::endl;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL_Init failed" << std::endl;
        return(r_delete(cart, cpu, ram, -3));
    }

    if (TTF_Init() < 0)
    {
        std::cout << "TTF_Init failed" << std::endl;
        return(r_delete(cart, cpu, ram, -4));
    }

    cpu->cpu_init();

    emu.running = 1;

    while (emu.running)
    {
        if (emu.paused)
        {
            SDL_Delay(10);
            continue;
        }
        try
        {
            cpu->cpu_step();
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return(r_delete(cart, cpu, ram, -6));
        }
        emu.ticks++;

        if (emu.ticks > 30)
            break ; // test
    }

    return (r_delete(cart, cpu, ram, 0));
}