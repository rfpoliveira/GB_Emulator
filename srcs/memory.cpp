
#include "../incs/main.hpp"

/*function called when the program is closed to free the memory*/
int close_emu(void)
{
    SDL_Quit();
    TTF_Quit();

    return (0);
}