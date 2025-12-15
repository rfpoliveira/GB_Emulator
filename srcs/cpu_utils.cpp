#include "../incs/cpu.hpp"

u16 reverse(u16 n) {
    return ((n & 0xFF00) >> 8) | ((n & 0x00FF) << 8);
}

/*returns the correct cpu register from the regist_type variable*/
u16 CPU::cpu_read_regis(regist_type rt) 
{
    switch(rt) 
    {
        case A: 
            return regis.a;
        case F: 
            return regis.f;
        case B: 
            return regis.b;
        case C: 
            return regis.c;
        case D: 
            return regis.d;
        case E: 
            return regis.e;
        case H: 
            return regis.h;
        case L: 
            return regis.l;

        case AF: 
            return reverse(*((u16 *)&regis.a));
        case BC: 
            return reverse(*((u16 *)&regis.b));
        case DE: 
            return reverse(*((u16 *)&regis.d));
        case HL: 
            return reverse(*((u16 *)&regis.h));

        case PC: 
            return regis.pc;
        case SP: 
            return regis.sp;
        default: 
            return 0;
    }
}

void CPU::cpu_set_regis(regist_type rt, u16 address) 
{
    switch(rt) 
    {
        case A: 
            regis.a = address & 0xFF; break;
        case F: 
            regis.f = address & 0xFF; break;
        case B: 
            regis.b = address & 0xFF; break;
        case C: 
            regis.c = address & 0xFF; break;
        case D: 
            regis.d = address & 0xFF; break;
        case E: 
            regis.e = address & 0xFF; break;
        case H: 
            regis.h = address & 0xFF; break;
        case L: 
            regis.l = address & 0xFF; break;

        case AF: 
            *((u16 *)&regis.a) = reverse(address); break;
        case BC: 
            *((u16 *)&regis.b) = reverse(address); break;
        case DE: 
            *((u16 *)&regis.d) = reverse(address); break;
        case HL: 
            *((u16 *)&regis.h) = reverse(address); break;

        case PC: 
            regis.pc = address; break;
        case SP: 
            regis.sp = address; break;
    }

    return ;
}

CPU::FUNC_PROC CPU::inst_get_proc(inst_type type)
{
    return(procs[type]);
};

bool CPU::check_conditional()
{
     bool z = CPU_FLAG_Z;
     bool c = CPU_FLAG_C;

     switch (curr_inst->cond)
     {
          case CT_NONE:
               return (true);
          case CT_C: 
               return (c);
          case CT_NC: 
               return(!c);
          case CT_Z:
               return(z);
          case CT_NZ:
               return(!z);
     }
     return (false);
}
/* sets the bits of the f register (flags) to the value passed*/
void CPU::cpu_flags(char z, char n, char h, char c)
{
    if (z != -1) {
        BIT_SET(regis.f, 7, z);
    }

    if (n != -1) {
        BIT_SET(regis.f, 6, n);
    }

    if (h != -1) {
        BIT_SET(regis.f, 5, h);
    }

    if (c != -1) {
        BIT_SET(regis.f, 4, c);
    }
}

