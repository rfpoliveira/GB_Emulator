#pragma once

#include "../incs/main.hpp"
#include "../incs/cart.hpp"
#include "../incs/emu.hpp"
#include "../incs/bus.hpp"
#include "../incs/instructions.hpp"
#include "../incs/ram.hpp"

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

class CPU
{
    public:
        CPU() {};
        CPU(Cart *cart, RAM *ram);
        ~CPU() {};

        int cpu_init(void);
        bool cpu_step(void);

        static u8 get_ie_register();
        static void set_ie_register(u8 address);
        static u8 ie_register;

    private:
        cpu_registers regis;
        u16 fetch_data;
        u16 mem_dest;
        bool dest_is_mem;
        u8 curr_opcode;
        instruction *curr_inst;
        Cart *cart_ptr;
        RAM *ram_ptr;

        bool halted;
        bool stepping;

        bool master_enabled;

        //registers
        u16 cpu_read_regis(regist_type rt);
        void cpu_set_regis(regist_type rt, u16 address);

        //stteping loop
        void fetch_instruction();
        void fetch_data_inst();
        void execute_inst();

        //intruction functions
        typedef std::function<void()> FUNC_PROC;
        FUNC_PROC procs[44];
        void proc_NONE();
        void proc_NOP();
        void proc_LD();
        void proc_INC();
        void proc_DEC();
        void proc_RLCA();
        void proc_ADD();
        void proc_ADC();
        void proc_RRCA();
        void proc_STOP();
        void proc_RLA();
        void proc_JR();
        void proc_RRA();
        void proc_DAA();
        void proc_CPL();
        void proc_SCF();
        void proc_CCF();
        void proc_HALT();
        void proc_SUB();
        void proc_SBC();
        void proc_AND();
        void proc_XOR();
        void proc_OR();
        void proc_CP();
        void proc_RET();
        void proc_RETI();
        void proc_POP();
        void proc_CALL();
        void proc_PUSH();
        void proc_RST();
        void proc_EI();
        void proc_DI();
        void proc_JP();
                 //CB instructions...
        void proc_RLC(); 
        void proc_RRC();
        void proc_RL(); 
        void proc_RR();
        void proc_SLA(); 
        void proc_RA();
        void proc_SWAP(); 
        void proc_SRL();
        void proc_BIT(); 
        void proc_RES(); 
        void proc_SET();
        //void proc_LDH();

        bool check_conditional();
        void goto_address(u16 address, bool push_pc);
        FUNC_PROC inst_get_proc(inst_type type);

        //flags
        void cpu_flags(char z, char n, char h, char c);
        #define CPU_FLAG_Z BIT(regis.f, 7);
        #define CPU_FLAG_C BIT(regis.f, 4);


        //stack
        void stack_push(u8 data);
        void stack_push16(u16 data);
        u8 stack_pop();
        u16 stack_pop16();

        //expection
        class CPUFailedExeption: public std::exception
        {
            public:
                virtual const char *what() const throw()
                {
                    return ("Error: CPU Stopped");
                }
        };
};