#pragma once

#include "main.hpp"

class Emulator
{
    public:
        Emulator();
        ~Emulator();
        bool running;
        bool paused;
        uint64_t ticks;
};