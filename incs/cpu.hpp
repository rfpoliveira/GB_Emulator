#pragma once

#include "main.hpp"
#include "instructions.hpp"

struct cpu_registers
{
    u8 a;
    u8 f;
    u8 b;
    u8 c;
    u8 d;
    u8 e;
    u8 h;
    u8 l;
    u16 pc;
    u16 sp;
};

struct CPU
{
        cpu_registers regis;
        u16 fetch_data;
        u16 mem_dest;
        u8 curr_opcode;
        instruction *curr_inst;

        bool halted;
        bool stepping;
};

int cpu_init(void);
int cpu_step(void);