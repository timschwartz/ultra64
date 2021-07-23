#include "vr4300.hpp"
#include "../opcodes.hpp"

using namespace ultra64;

void vr4300::special(vr4300 *cpu)
{
    opcode_special_type op(cpu->current_instruction);

    cpu->opcode_special[op.function](cpu);
}

void vr4300::sll(vr4300 *cpu)
{
    opcode_special_type op(cpu->current_instruction);

    cpu->GPR[op.rd] = (cpu->GPR[op.rt] & 0xFFFFFFFF) << op.sa;
    cpu->PC += 4;
}

void vr4300::srl(vr4300 *cpu)
{
    opcode_special_type op(cpu->current_instruction);

    cpu->GPR[op.rd] = (cpu->GPR[op.rt] & 0xFFFFFFFF) >> op.sa;
    cpu->PC += 4;
}

void vr4300::jr(vr4300 *cpu)
{
    opcode_special_type op(cpu->current_instruction);

    uint32_t branch = cpu->GPR[op.rs];
    cpu->PC += 4;
    cpu->step();
    cpu->PC = branch;
}

void vr4300::_or(vr4300 *cpu)
{
    opcode_special_type op(cpu->current_instruction);

    cpu->GPR[op.rd] = cpu->GPR[op.rs] | cpu->GPR[op.rt];
    cpu->PC += 4;
}

void vr4300::dsll32(vr4300 *cpu)
{
    opcode_special_type op(cpu->current_instruction);
    
    cpu->GPR[op.rd] = cpu->GPR[op.rt] << (op.sa + 32);
    cpu->PC += 4;
}
