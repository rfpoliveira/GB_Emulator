#include "../incs/cpu.hpp"

/*only inicialized the variables (safety)*/
CPU::CPU(Cart *cart, RAM *ram)
{
    regis.a = 0x01; //this are the default values for the registers
    regis.f = 0x00;
    regis.b = 0x00;
    regis.c = 0x00;
    regis.d = 0x00;
    regis.e = 0x00;
    regis.h = 0x00;
    regis.l = 0x00;
    regis.pc = 0;
    regis.sp = 0x00;
    fetch_data = 0;
    mem_dest = 0;
    dest_is_mem = false;
    curr_opcode = 0;
    curr_inst = 0;
    halted = false;
    stepping = false;
    master_enabled = true;

    cart_ptr = cart;
    ram_ptr = ram;

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
    //procs[44] = std::bind(&CPU::proc_LDH, this);
};

int CPU::cpu_init(void)
{
    regis.pc = 0x100; // << entry point for the cpu instructions
    return (0);
}

/*reads from the rom_data the curr instructions*/
void CPU::fetch_instruction()
{
    curr_opcode = bus_read(regis.pc++, cart_ptr, ram_ptr);
    curr_inst = fetch_instruction_by_opcode(curr_opcode);
    if (!curr_inst)
    {
        std::cerr << "Unknow insctruction: " << std::hex << curr_inst << '\n';
        std::cerr << " at pc: " << std::hex << regis.pc << '\n';
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

std::string get_type(enum inst_type type) //just for debug
{
    switch (type)
    {
        case IT_NONE: return ("IT_NONE");
        case NOP: return ("NOP");
        case LD: return ("LD");
        case INC: return ("INC");
        case DEC: return ("DEC");
        case RLCA: return ("RLCA");
        case ADD: return ("ADD");
        case ADC: return ("ADC");
        case RRCA: return ("RRCA");
        case STOP: return ("STOP");
        case RLA: return ("RLA");
        case JR: return ("JR");
        case RRA: return ("RRA");
        case DAA: return ("DAA");
        case CPL: return ("CPL");
        case SCF: return ("SCF");
        case CCF: return ("CCF");
        case HALT: return ("HALT");
        case SUB: return ("SUB");
        case SBC: return ("SBC");
        case AND: return ("AND");
        case XOR: return ("XOR");
        case OR: return ("OR");
        case CP: return ("CP");
        case RET: return ("RET");
        case RETI: return ("RETI");
        case POP: return ("POP");
        case CALL: return ("CALL");
        case PUSH: return ("PUSH");
        case RST: return ("RST");
        case EI: return ("EI");
        case DI: return ("DI");
        case JP: return ("JP");
        case RLC: return ("RLC"); 
        case RRC: return ("RRC");
        case RL: return ("RL"); 
        case RR: return ("RR");
        case SLA: return ("SLA"); 
        case RA: return ("RA");
        case SWAP: return ("SWAP"); 
        case SRL: return ("SRL");
        case BIT: return ("BIT"); 
        case RES: return ("RES"); 
        case SET: return ("SET");
        //case LDH: return ("LDH"); 
    }
    return (nullptr);
}

/*the real cpu loop going threw the instructions*/
bool CPU::cpu_step(void)
{
    u16 pc_og = regis.pc; //for the debug print

    if(!halted)
    { 
        dest_is_mem = false; //back to the normal state
        fetch_instruction(); //gets the insctruction from the rom data and puts it into the curr_inst
        fetch_data_inst(); /*puts on fecth_data the correct data depending on the address mode of the instruction*/


        //debug_print
        printf("%04X: %s (%02X %02X %02X) A: %02X BC: %02X%02X DE: %02X%02X HL: %02X%02X\n", 
            pc_og, get_type(curr_inst->type).c_str(), curr_opcode,
            bus_read(pc_og + 1, cart_ptr, ram_ptr), bus_read(pc_og + 2, cart_ptr, ram_ptr), 
            regis.a, regis.b, regis.c, regis.d, regis.e, regis.h, regis.l);


        execute_inst(); /*finnaly executes the instruction*/
    }
    return (true);
}