#include "vr4300.hpp"
#include "../opcodes.hpp"
#include "../instruction.hpp"

#include <sstream>
#include <iostream>

using namespace ultra64;

vr4300::vr4300(MMU &mmu)
{
    this->mmu = &mmu;

    for(int i = 0; i < 0x40; i++)
    {
        this->opcode[i] = this->opcode_cp0[i] = this->opcode_special[i] = this->opcode_regimm[i] = vr4300::not_implemented;
    }

    this->opcode[_SPECIAL] = vr4300::special;
    this->opcode[_REGIMM] = vr4300::regimm;
    this->opcode[BEQ] = vr4300::beq;
    this->opcode[BNE] = vr4300::bne;
    this->opcode[ADDI] = vr4300::addi;
    this->opcode[ADDIU] = vr4300::addiu;
    this->opcode[ANDI] = vr4300::andi;
    this->opcode[ORI] = vr4300::ori;
    this->opcode[LUI] = vr4300::lui;
    this->opcode[_CP0] = vr4300::cp0;
    this->opcode[BEQL] = vr4300::beql;
    this->opcode[BNEL] = vr4300::bnel;
    this->opcode[DADDI] = vr4300::daddi;
    this->opcode[LW] = vr4300::lw;
    this->opcode[SW] = vr4300::sw;

    this->opcode_special[SLL] = vr4300::sll;
    this->opcode_special[SRL] = vr4300::srl;
    this->opcode_special[SLLV] = vr4300::sllv;
    this->opcode_special[SRLV] = vr4300::srlv;
    this->opcode_special[JR] = vr4300::jr;
    this->opcode_special[MFHI] = vr4300::mfhi;
    this->opcode_special[MFLO] = vr4300::mflo;
    this->opcode_special[MULTU] = vr4300::multu;
    this->opcode_special[ADDU] = vr4300::addu;
    this->opcode_special[SUBU] = vr4300::subu;
    this->opcode_special[AND] = vr4300::_and;
    this->opcode_special[OR] = vr4300::_or;
    this->opcode_special[XOR] = vr4300::_xor;
    this->opcode_special[SLTU] = vr4300::sltu;
    this->opcode_special[DSLL32] = vr4300::dsll32;

    this->opcode_regimm[BLTZ] = vr4300::bltz;
    this->opcode_regimm[BGEZAL] = vr4300::bgezal;

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
    std::cout << std::dec << this->CP0[9] << " - " << std::hex << this->PC << ": " << i.to_string() << std::endl;
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

void vr4300::beq(vr4300 *cpu)
{
    opcode_i_type op(cpu->current_instruction);

    cpu->PC += 4;

    if(cpu->GPR[op.rs] == cpu->GPR[op.rt])
    {
        uint32_t branch = cpu->PC + (((int16_t)op.immediate) << 2);
        std::cout << "    BEQ: " << std::hex << cpu->GPR[op.rs] << " is equal to " << cpu->GPR[op.rt] << std::endl;
        try
        {
            cpu->step();
        }
        catch(std::runtime_error &e)
        {
            throw e;
        }

        std::cout << "    BEQ branching to " << branch << std::endl;
        cpu->PC = branch;
    }
}

void vr4300::bne(vr4300 *cpu)
{
    opcode_i_type op(cpu->current_instruction);

    cpu->PC += 4;

    if(cpu->GPR[op.rs] != cpu->GPR[op.rt]) 
    {
        uint32_t branch = cpu->PC + (((int16_t)op.immediate) << 2);
        std::cout << "    BNE: " << std::hex << cpu->GPR[op.rs] << " is not equal to " << cpu->GPR[op.rt] << std::endl;

        try
        {
            cpu->step(); 
        }
        catch(std::runtime_error &e)
        {
            throw e;
        }

        std::cout << "    BNE branching to " << branch << std::endl;
        cpu->PC = branch;
    }
}

void vr4300::addi(vr4300 *cpu)
{
    opcode_i_type op(cpu->current_instruction);
    cpu->GPR[op.rt] = cpu->GPR[op.rs] + (int16_t)(op.immediate);
    cpu->PC += 4;
    std::cerr << "ADDI: overflow trap not implemented" << std::endl;
    // Make an exception for this
}

void vr4300::addiu(vr4300 *cpu)
{
    opcode_i_type op(cpu->current_instruction);
    cpu->GPR[op.rt] = cpu->GPR[op.rs] + (int16_t)(op.immediate);
    cpu->PC += 4;
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

    cpu->PC += 4;

    uint32_t branch = cpu->PC + ((int16_t)op.immediate << 2);
        
    if(cpu->GPR[op.rs] == cpu->GPR[op.rt])
    {
        std::cout << "    BEQL " << std::hex << cpu->GPR[op.rs] << " is equal to " << cpu->GPR[op.rt] << std::endl;
        try
        {
            cpu->step();
        }
        catch(std::runtime_error &e)
        {
            throw e;
        }

        std::cout << "    BEQL branching to " << branch << std::endl;

        cpu->PC = branch;
    }
    else cpu->PC += 4;
}

void vr4300::bnel(vr4300 *cpu)
{
    opcode_i_type op(cpu->current_instruction);
    cpu->PC += 4;

    if(cpu->GPR[op.rs] != cpu->GPR[op.rt])
    {
        uint32_t branch = cpu->PC + ((int16_t)op.immediate << 2);
        std::cout << "    BNEL " << std::hex << cpu->GPR[op.rs] << " is not equal to " << cpu->GPR[op.rt] << std::endl;
        try
        {
            cpu->step();
        }
        catch(std::runtime_error &e)
        {
            throw e;
        }
        
        cpu->PC = branch;
        std::cout << "    BNEL branching to 0x" << std::hex << cpu->PC << std::endl;
    }
    else cpu->PC += 4;
}

void vr4300::daddi(vr4300 *cpu)
{
    opcode_i_type op(cpu->current_instruction);
    cpu->GPR[op.rt] = (int64_t)cpu->GPR[op.rs] + (int32_t)(int16_t)(op.immediate);
    cpu->PC += 4;
}

void vr4300::lw(vr4300 *cpu)
{
    opcode_i_type op(cpu->current_instruction);
    cpu->GPR[op.rt] = cpu->mmu->read_word(cpu->GPR[op.rs] + op.immediate);
    std::cout << "    Read 0x" << std::hex << cpu->GPR[op.rt] << " from 0x" << (cpu->GPR[op.rs] + (int16_t)op.immediate) << std::endl;
    cpu->PC += 4;
}

void vr4300::sw(vr4300 *cpu)
{
    opcode_i_type op(cpu->current_instruction);
    cpu->mmu->write_word(cpu->GPR[op.rs] + (int16_t)op.immediate, (cpu->GPR[op.rt] & 0xFFFFFFFF));
    std::cout << "    Wrote 0x" << std::hex << (cpu->GPR[op.rt] & 0xFFFFFFFF) << " to 0x" << (cpu->GPR[op.rs] + (int16_t)op.immediate) << std::endl;
    cpu->PC += 4;
}
