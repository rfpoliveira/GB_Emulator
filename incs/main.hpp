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

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;


int run_emu(int argc, char** argv);
bool args_parse(int argc, char **argv);
int close_emu(void);