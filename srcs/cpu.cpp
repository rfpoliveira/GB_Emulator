#include "../incs/cpu.hpp"

extern Emulator emu;

/*only inicialized the variables (safety)*/
CPU::CPU(Cart *cart)
{
    regis.a = 0x01; //this are the default values for the registers
    regis.f = 0xB0;
    regis.b = 0x00;
    regis.c = 0x13;
    regis.d = 0x00;
    regis.e = 0xD8;
    regis.h = 0x01;
    regis.l = 0x4D;
    regis.pc = 0;
    regis.sp = 0xFFFE;
    fetch_data = 0;
    mem_dest = 0;
    dest_is_mem = false;
    curr_opcode = 0;
    curr_inst = 0;
    halted = false;
    stepping = false;
    master_enabled = true;
    cart_ptr = cart;

    procs[0] = std::bind(&CPU::proc_NONE, this);
    procs[1] = std::bind(&CPU::proc_NOP, this);
    procs[2] = std::bind(&CPU::proc_LD, this);
    procs[3] = std::bind(&CPU::proc_INC, this);
    procs[4] = std::bind(&CPU::proc_DEC, this);
    procs[5] = std::bind(&CPU::proc_RLCA, this);
    procs[6] = std::bind(&CPU::proc_ADD, this);
    procs[7] = std::bind(&CPU::proc_ADC, this);
    procs[8] = std::bind(&CPU::proc_RRCA, this);
    procs[9] = std::bind(&CPU::proc_STOP, this);
    procs[10] = std::bind(&CPU::proc_RLA, this);
    procs[11] = std::bind(&CPU::proc_JR, this);
    procs[12] = std::bind(&CPU::proc_RRA, this);
    procs[13] = std::bind(&CPU::proc_DAA, this);
    procs[14] = std::bind(&CPU::proc_CPL, this);
    procs[15] = std::bind(&CPU::proc_SCF, this);
    procs[16] = std::bind(&CPU::proc_CCF, this);
    procs[17] = std::bind(&CPU::proc_HALT, this);
    procs[18] = std::bind(&CPU::proc_SUB, this);
    procs[19] = std::bind(&CPU::proc_SBC, this);
    procs[20] = std::bind(&CPU::proc_AND, this);
    procs[21] = std::bind(&CPU::proc_XOR, this);
    procs[22] = std::bind(&CPU::proc_OR, this);
    procs[23] = std::bind(&CPU::proc_CP, this);
    procs[24] = std::bind(&CPU::proc_RET, this);
    procs[25] = std::bind(&CPU::proc_RETI, this);
    procs[26] = std::bind(&CPU::proc_POP, this);
    procs[27] = std::bind(&CPU::proc_CALL, this);
    procs[28] = std::bind(&CPU::proc_PUSH, this);
    procs[29] = std::bind(&CPU::proc_RST, this);
    procs[30] = std::bind(&CPU::proc_EI, this);
    procs[31] = std::bind(&CPU::proc_DI, this);
    procs[32] = std::bind(&CPU::proc_JP, this);
                 //CB instructions...
    procs[33] = std::bind(&CPU::proc_RLC, this); 
    procs[34] = std::bind(&CPU::proc_RRC, this);
    procs[35] = std::bind(&CPU::proc_RL, this); 
    procs[36] = std::bind(&CPU::proc_RR, this);
    procs[37] = std::bind(&CPU::proc_SLA, this); 
    procs[38] = std::bind(&CPU::proc_RA, this);
    procs[39] = std::bind(&CPU::proc_SWAP, this); 
    procs[40] = std::bind(&CPU::proc_SRL, this);
    procs[41] = std::bind(&CPU::proc_BIT, this); 
    procs[42] = std::bind(&CPU::proc_RES, this); 
    procs[43] = std::bind(&CPU::proc_SET, this);
};

int CPU::cpu_init(void)
{
    regis.pc = 0x100; // << entry point for the cpu instructions
    return (0);
}

/*reads from the rom_data the curr instructions*/
void CPU::fetch_instruction()
{
    curr_opcode = bus_read(regis.pc++, cart_ptr);
    curr_inst = fetch_instruction_by_opcode(curr_opcode);
    if (!curr_inst)
    {
        std::cerr << "Unknow insctruction: " << std::hex << curr_inst << '\n';
        std::cerr << " at pc: " << std::hex << regis.pc << '\n';
        throw CPUFailedExeption();
    }
}

/*puts on fecth_data the correct data depending on the adress mode of the instruction*/
void CPU::fetch_data_inst()
{
    switch (curr_inst->mode)
    {
        case AM_IMP:
            return;
        
        case AM_R: 
            fetch_data = cpu_read_regis(curr_inst->reg1); 
            return;

        case AM_R_D8:
            fetch_data = bus_read(regis.pc, cart_ptr);
            emu.emu_cycle(1);
            regis.pc++;
            return;

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

        default:
            printf("adress mode not implemented yet: %X\n", curr_opcode);
            throw CPUFailedExeption();
    }
}

void CPU::execute_inst()
{
    FUNC_PROC proc = inst_get_proc(curr_inst->type);
    if(!proc)
        throw CPUFailedExeption();
    proc();
}
/*the real cpu loop going threw the instructions*/

bool CPU::cpu_step(void)
{
    if(!halted)
    {
        fetch_instruction();
        printf("executing: %X at pc: %X\n", curr_opcode, regis.pc - 1);
        fetch_data_inst();
        execute_inst();
    }
    return (true);
}