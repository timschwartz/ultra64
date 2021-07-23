#include "vr4300.hpp"
#include "../opcodes.hpp"

using namespace ultra64;

void vr4300::regimm(vr4300 *cpu)
{
    opcode_regimm_type op(cpu->current_instruction);

    cpu->opcode_regimm[op.rt](cpu);
}

void vr4300::bltz(vr4300 *cpu)
{
    opcode_regimm_type op(cpu->current_instruction);
    
    cpu->PC += 4;
    if((signed)cpu->GPR[op.rs] < 0)
    {
        try
        {
            cpu->step();
        }
        catch(std::runtime_error &e)
        {
            throw e;
        }

        cpu->PC += op.offset;
        std::cout << "BLTZ: Jumping to " << std::hex << cpu->PC << std::endl;
    }
}

void vr4300::bgezal(vr4300 *cpu)
{
    opcode_regimm_type op(cpu->current_instruction);

    cpu->PC += 4;
    if((signed)cpu->GPR[op.rs] >= 0)
    {
        cpu->GPR[31] = cpu->PC + 4;

        try
        {
            cpu->step();
        }
        catch(std::runtime_error &e)
        {
            throw e;
        }

        cpu->PC += op.offset;
        std::cout << "bgezal: Jumping to " << std::hex << cpu->PC << std::endl;
    }
}
