#include "vr4300.hpp"
#include "../opcodes.hpp"

void vr4300::mtc0(vr4300 *cpu)
{
    opcode_co_type op(cpu->current_instruction);

    cpu->CP0[op.rd] = cpu->GPR[op.rt];
    cpu->PC += 4;
}
