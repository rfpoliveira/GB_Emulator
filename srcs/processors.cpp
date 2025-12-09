#include "../incs/cpu.hpp"

extern Emulator emu;

void CPU::proc_NONE()
{
     std::cerr << "Error: Invalid instruction\n";
     throw CPUFailedExeption();
};
void CPU::proc_NOP(){};

void CPU::proc_LD(){};
void CPU::proc_INC(){};
void CPU::proc_DEC(){};
void CPU::proc_RLCA(){};
void CPU::proc_ADD(){};
void CPU::proc_ADC(){};
void CPU::proc_RRCA(){};
void CPU::proc_STOP(){};
void CPU::proc_RLA(){};
void CPU::proc_JR(){};
void CPU::proc_RRA(){};
void CPU::proc_DAA(){};
void CPU::proc_CPL(){};
void CPU::proc_SCF(){};
void CPU::proc_CCF(){};
void CPU::proc_HALT(){};
void CPU::proc_SUB(){};
void CPU::proc_SBC(){};
void CPU::proc_AND(){};

void CPU::proc_XOR()
{
     regis.a ^= fetch_data & 0xFF;
     cpu_flags(regis.a == 0, 0, 0, 0);
};

void CPU::proc_OR(){};
void CPU::proc_CP(){};
void CPU::proc_RET(){};
void CPU::proc_RETI(){};
void CPU::proc_POP(){};
void CPU::proc_CALL(){};
void CPU::proc_PUSH(){};
void CPU::proc_RST(){};
void CPU::proc_EI(){};

void CPU::proc_DI()
{
     master_enabled = false;
};

void CPU::proc_JP()
{
     if(check_conditional())
     {
          regis.pc = fetch_data;
          emu.emu_cycle(1);
     }
};

     //CB instructions...
void CPU::proc_RLC(){}; 
void CPU::proc_RRC(){};
void CPU::proc_RL(){}; 
void CPU::proc_RR(){};
void CPU::proc_SLA(){}; 
void CPU::proc_RA(){};
void CPU::proc_SWAP(){}; 
void CPU::proc_SRL(){};
void CPU::proc_BIT(){}; 
void CPU::proc_RES(){}; 
void CPU::proc_SET(){};
