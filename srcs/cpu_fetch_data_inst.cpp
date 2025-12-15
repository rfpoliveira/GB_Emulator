#include "../incs/cpu.hpp"

extern Emulator emu;

/*puts on fecth_data the correct data we will need for the insctruction depending on the adress mode of the instruction*/
void CPU::fetch_data_inst()
{
    switch (curr_inst->mode)
    {
        case AM_IMP:
            return;
        
        case AM_R: //reads memery from a register
            fetch_data = cpu_read_regis(curr_inst->reg1); 
            return;

        case AM_R_R: //reas memory from the second register
            fetch_data = cpu_read_regis(curr_inst->reg2); 
            return;

        case AM_R_D8: //reads a 8bit adress from pc
            fetch_data = bus_read(regis.pc, cart_ptr);
            emu.emu_cycle(1);
            regis.pc++;
            return;

        case AM_R_D16: //reads a 16bit adress from pc
        case AM_D16:
        {
            u16 low = bus_read(regis.pc, cart_ptr);
            emu.emu_cycle(1);
            u16 high = bus_read(regis.pc + 1, cart_ptr);
            emu.emu_cycle(1);

            fetch_data = low | (high << 8);
            regis.pc += 2;
            return;
        }

        case AM_MR_R: //reads from the first regester and the destination will be the first
        {
            fetch_data = cpu_read_regis(curr_inst->reg2);
            mem_dest = cpu_read_regis(curr_inst->reg1);
            dest_is_mem = true;

            if (curr_inst->reg1 == C) //the c resgister as special behavior in this case (check docs)
                mem_dest |= 0xFF00; //sets the high bit to FF and as no effect on the low bit
            return ;
        }

        case AM_R_MR: //reads from an adress and the destination will be the register
        {
            u16 address = cpu_read_regis(curr_inst->reg2);
            if (curr_inst->reg1 == C)
                address |= 0xFF00;
            fetch_data = bus_read(address, cart_ptr);
            emu.emu_cycle(1);
            return ;
        }

        case AM_R_HLI: //reads from HL and increments it
        {
            fetch_data = bus_read(cpu_read_regis(curr_inst->reg2), cart_ptr);
            emu.emu_cycle(1);
            cpu_set_regis(HL, cpu_read_regis(HL) + 1);
            return ;
        }

        case AM_R_HLD: //reads from HL and decrincrements it
        {
            fetch_data = bus_read(cpu_read_regis(curr_inst->reg2), cart_ptr);
            emu.emu_cycle(1);
            cpu_set_regis(HL, cpu_read_regis(HL) - 1);
            return ;
        }

        case AM_HLI_R: //reads from regs2, sets hl to that and increments it
        {
            fetch_data = cpu_read_regis(curr_inst->reg2);
            mem_dest = cpu_read_regis(curr_inst->reg1);
            dest_is_mem = true;
            cpu_set_regis(HL, cpu_read_regis(HL) + 1);
            return ;
        }

        case AM_HLD_R: //reads from regs2, sets hl to that and decrincrements it
        {
            fetch_data = cpu_read_regis(curr_inst->reg2);
            mem_dest = cpu_read_regis(curr_inst->reg1);
            dest_is_mem = true;
            cpu_set_regis(HL, cpu_read_regis(HL) - 1);
            return ;
        }

        case AM_R_A8: //reads from the program counter
        {
            fetch_data = bus_read(regis.pc, cart_ptr);
            emu.emu_cycle(1);
            regis.pc++;
            return  ;
        }


        case AM_A8_R: //reads to the program counter
        {
            mem_dest = bus_read(regis.pc, cart_ptr) | 0xFF00;
            dest_is_mem = true;
            emu.emu_cycle(1);
            regis.pc++;
            return ;
        }

        case AM_HL_SPR: //load the stack pointer into HL and increment by r8
        {
            fetch_data = bus_read(regis.pc, cart_ptr);
            emu.emu_cycle(1);
            regis.pc++;
            return ;
        }

        case AM_D8:
        {
            fetch_data = bus_read(regis.pc, cart_ptr);
            emu.emu_cycle(1);
            regis.pc++;
            return ;
        }

        case AM_A16_R:
        case AM_D16_R: //moving from a register into a 16bit adress
        {
            u16 low = bus_read(regis.pc, cart_ptr);
            emu.emu_cycle(1);

            u16 high = bus_read(regis.pc + 1, cart_ptr);
            emu.emu_cycle(1);

            mem_dest = low | high << 8;
            dest_is_mem = true;

            regis.pc += 2;
            fetch_data = cpu_read_regis(curr_inst->reg2);
        }

        case AM_MR_D8: //moving a 8bit adress to the adress of a register
        {
            fetch_data = bus_read(regis.pc, cart_ptr);
            emu.emu_cycle(1);
            regis.pc++;
            mem_dest = cpu_read_regis(curr_inst->reg1);
            dest_is_mem = true;
            return ;
        }

        case AM_MR:
        {
            mem_dest = cpu_read_regis(curr_inst->reg1);
            dest_is_mem = true;
            fetch_data = bus_read(cpu_read_regis(curr_inst->reg1), cart_ptr);
            emu.emu_cycle(1);
            return ;
        }

        case AM_R_A16:
        {
            u16 low = bus_read(regis.pc, cart_ptr);
            emu.emu_cycle(1);
            u16 high = bus_read(regis.pc + 1, cart_ptr);
            emu.emu_cycle(1);

            u16 address = low | (high << 8);
            regis.pc += 2;

            fetch_data = bus_read(address, cart_ptr);
            emu.emu_cycle(1);
            return;
        }

        default:
            printf("adress mode not implemented yet: %X\n", curr_opcode);
            throw CPUFailedExeption();
    }
}