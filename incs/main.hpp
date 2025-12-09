#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <exception>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <memory>
#include <functional>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

#define BIT(a, n) ((a & (1 << n)) ? 1 : 0) //changes bit n of register a

#define BIT_SET(a, n, on) (on ? (a) |= (1 << n) : (a) &= ~(1 << n)) //changes bit n of register a comparin it to on

#define BETWEEN(a, b, c) ((a >= b) && (a <= c))

int run_emu(int argc, char** argv);
bool args_parse(int argc, char **argv);
int close_emu(void);