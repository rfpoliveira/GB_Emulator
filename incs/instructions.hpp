#pragma once

#include "main.hpp"

enum cond_type
{
    CT_NONE, 
    CT_NZ, 
    CT_Z, 
    CT_NC, 
    CT_C
};

enum addr_mode
{
    AM_IMP,
    AM_R_D16,
    AM_R_R,
    AM_MR_R,
    AM_R,
    AM_R_D8,
    AM_R_MR,
    AM_R_HLI,
    AM_R_HLD,
    AM_HLI_R,
    AM_HLD_R,
    AM_R_A8,
    AM_A8_R,
    AM_HL_SPR,
    AM_D16,
    AM_D8,
    AM_D16_R,
    AM_MR_D8,
    AM_MR,
    AM_A16_R,
    AM_R_A16
};

enum inst_type
{
    IT_NONE,
    NOP,
    LD,
    INC,
    DEC,
    RLCA,
    ADD,
    ADC,
    RRCA,
    STOP,
    RLA,
    JR,
    RRA,
    DAA,
    CPL,
    SCF,
    CCF,
    HALT,
    SUB,
    SBC,
    AND,
    XOR,
    OR,
    CP,
    RET,
    RETI,
    POP,
    CALL,
    PUSH,
    RST,
    EI,
    DI,
    JP,
        //CB instructions...
    RLC, 
    RRC,
    RL, 
    RR,
    SLA, 
    RA,
    SWAP, 
    SRL,
    BIT, 
    RES, 
    SET
};

enum regist_type{
    RT_NONE,
    A,
    F,
    B,
    C,
    D,
    E,
    H,
    L,
    AF,
    BC,
    DE,
    HL,
    SP,
    PC
};

struct instruction
{
    inst_type type;
    addr_mode mode;
    regist_type reg1;
    regist_type reg2;
    cond_type cond;
    u8 param;
};

instruction *fetch_instruction_by_opcode(u8 opcode);