#include "../incs/cpu.hpp"

extern Emulator emu;

void CPU::proc_NONE()
{
     std::cerr << "Error: Invalid instruction\n";
     throw CPUFailedExeption();
};
void CPU::proc_NOP(){};

void CPU::proc_LD()
{
     if (dest_is_mem)
     {
          if (curr_inst->reg2 >= AF) //if it is a 16bit register
          {
               emu.emu_cycle(1);
               bus_write_16bit(mem_dest, fetch_data, cart_ptr, ram_ptr);
          }
          else //8bit register
               bus_write(mem_dest, fetch_data, cart_ptr, ram_ptr);

          emu.emu_cycle(1);
          return ;
     }

     if (curr_inst->mode == AM_HL_SPR)
     {
          u8 hflag = (cpu_read_regis(curr_inst->reg2) & 0xF) + 
                    (fetch_data & 0xF) >= 0x10;
          u8 cflag = (cpu_read_regis(curr_inst->reg2) & 0xFF) + 
                    (fetch_data & 0xFF) >= 0x100;
          cpu_flags(0, 0, hflag, cflag);
          cpu_set_regis(curr_inst->reg1, 
               cpu_read_regis(curr_inst->reg2) + (char)fetch_data); //casting cuz there could be a negative number

          return ;
     }
     cpu_set_regis(curr_inst->reg1, fetch_data);
};

void CPU::proc_INC(){};
void CPU::proc_DEC(){};
void CPU::proc_RLCA(){};
void CPU::proc_ADD(){};
void CPU::proc_ADC(){};
void CPU::proc_RRCA(){};
void CPU::proc_STOP(){};
void CPU::proc_RLA(){};

void CPU::proc_JR()
{
     char relative = (char)(fetch_data & 0xFF); //take the low bit and cast it cuz it can be negative (jumping back)
     u16 address = regis.pc + relative;
     goto_address(address, false);
};
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

void CPU::proc_RET()
{
     if (curr_inst->cond != CT_NONE)
          emu.emu_cycle(1);
     if(check_conditional())
     {
          u16 low = stack_pop();
          emu.emu_cycle(1);
          u16 high = stack_pop();
          emu.emu_cycle(1);

          u16 value = (high << 8) | low;
          regis.pc = value;
          emu.emu_cycle(1);
     }
};
void CPU::proc_RETI()
{
     master_enabled = true;
     proc_RET();
};

void CPU::proc_POP()
{
     u16 low = stack_pop();
     emu.emu_cycle(1);
     u16 high = stack_pop();
     emu.emu_cycle(1);

     u16 value = (high << 8) | low;

     cpu_set_regis(curr_inst->reg1, value);

     if (curr_inst->reg1 == AF) //special case with this register we only grab 1.5 bytes
          cpu_set_regis(curr_inst->reg1, value & 0xFFF0);
};
void CPU::proc_CALL()
{
     goto_address(fetch_data, true);
};

void CPU::proc_PUSH()
{
     u16 high = (cpu_read_regis(curr_inst->reg1) >> 8) & 0xFF;
     emu.emu_cycle(1);
     stack_push(high);

     u16 low = cpu_read_regis(curr_inst->reg2) & 0xFF;
     emu.emu_cycle(1);
     stack_push(low);

     emu.emu_cycle(1);
};
void CPU::proc_RST()
{
     goto_address(curr_inst->param, true);
};
void CPU::proc_EI(){};

void CPU::proc_DI()
{
     master_enabled = false;
};

void CPU::proc_JP()
{
     goto_address(fetch_data, false);
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

/* void CPU::proc_LDH()
{    
     if (curr_inst->reg1 == A)
          cpu_set_regis(curr_inst->reg1, bus_read(0xFF00 | fetch_data, cart_ptr, ram_ptr));
     else
          bus_write(0xFF00 | fetch_data, regis.a, cart_ptr, ram_ptr);
     emu.emu_cycle(1);
}; */
