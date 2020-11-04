#include "vr4300.hpp"
#include "../opcodes.hpp"
#include "../instruction.hpp"

#include <sstream>
#include <iostream>

using namespace ultra64;

vr4300::vr4300(MMU &mmu)
{
    this->mmu = &mmu;

    for(int i; i < 40; i++)
    {
        this->opcode[i] = this->opcode_cp0[i] = vr4300::not_implemented;
    }

    this->opcode[ANDI] = vr4300::andi;
    this->opcode[ORI] = vr4300::ori;
    this->opcode[LUI] = vr4300::lui;
    this->opcode[_CP0] = vr4300::cp0;
    this->opcode[BEQL] = vr4300::beql;
    this->opcode[LW] = vr4300::lw;

    this->opcode_cp0[MTC0] = vr4300::mtc0;

    this->PC = 0x1FC00000;
}

uint32_t vr4300::get_PC()
{
    return this->PC;
}

void vr4300::step()
{
    this->current_instruction = mmu->read_word(this->PC);
    opcode_i_type icode(this->current_instruction);
    instruction i(this->current_instruction);
    std::cout << i.to_string() << std::endl;
    this->opcode[icode.op](this);
    this->CP0[9] += 2;
}

void vr4300::not_implemented(vr4300 *cpu)
{
    instruction i(cpu->current_instruction);

    std::stringstream ss;
    ss << "0x" << std::hex << cpu->get_PC() << ": NOT IMPLEMENTED " << i.to_string();
    throw std::runtime_error(ss.str());
}

void vr4300::andi(vr4300 *cpu)
{
    opcode_i_type op(cpu->current_instruction);
    cpu->GPR[op.rt] = cpu->GPR[op.rs] & (uint64_t)(op.immediate);
    cpu->PC += 4;
}

void vr4300::ori(vr4300 *cpu)
{
    opcode_i_type op(cpu->current_instruction);
    cpu->GPR[op.rt] = cpu->GPR[op.rs] | (uint64_t)(op.immediate);
    cpu->PC += 4;
}

void vr4300::lui(vr4300 *cpu)
{
    opcode_i_type op(cpu->current_instruction);
    cpu->GPR[op.rt] = (uint64_t)(op.immediate) * 0x10000;
    cpu->PC += 4;
}

void vr4300::cp0(vr4300 *cpu)
{
    opcode_co_type op(cpu->current_instruction);
    cpu->opcode_cp0[op.sub](cpu);
}

void vr4300::beql(vr4300 *cpu)
{
    opcode_i_type op(cpu->current_instruction);
    int32_t branch_offset = ((int16_t)op.immediate) << 2;
    uint32_t branch = cpu->PC + branch_offset;
        
    if(cpu->GPR[op.rs] == cpu->GPR[op.rt]) cpu->PC = branch;
    else cpu->PC += 4;
}

void vr4300::lw(vr4300 *cpu)
{
    opcode_i_type op(cpu->current_instruction);
    cpu->GPR[op.rt] = cpu->mmu->read_word(cpu->GPR[op.rs] + op.immediate);
    cpu->PC += 4;
}
