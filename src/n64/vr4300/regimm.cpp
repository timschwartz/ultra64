#include "vr4300.hpp"
#include "../opcodes.hpp"

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
        std::cout << "    BLTZ " << std::hex << (signed)cpu->GPR[op.rs] << " is less than 0" << std::endl;
        uint32_t branch = cpu->PC + (((int16_t)op.offset) << 2);
        try
        {
            cpu->step();
        }
        catch(std::runtime_error &e)
        {
            throw e;
        }

        cpu->PC = branch;
        std::cout << "    BLTZ branching to " << std::hex << cpu->PC << std::endl;
    }
}

void vr4300::bgezal(vr4300 *cpu)
{
    opcode_regimm_type op(cpu->current_instruction);

    cpu->PC += 4;
    cpu->GPR[31] = cpu->PC + 4;
    if((signed)cpu->GPR[op.rs] >= 0)
    {
        std::cout << "    BGEZAL " << std::hex << (signed)cpu->GPR[op.rs] << " is greater than 0" << std::endl;
        uint32_t branch = cpu->PC + (((int16_t)op.offset) << 2);
        try
        {
            cpu->step();
        }
        catch(std::runtime_error &e)
        {
            throw e;
        }

        cpu->PC = branch;
        std::cout << "    BGEZAL branching to " << std::hex << branch << std::endl;
    }
}
