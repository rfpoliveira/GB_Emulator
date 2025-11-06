#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef uint8_t u8;
typedef uint16_t u16;

int run_emu(int argc, char** argv);
int close_emu(void);