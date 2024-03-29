#include "instruction.hpp"
#include "opcodes.hpp"

#include <sstream>
#include <iostream>

using namespace ultra64;

auto const upper = std::uppercase;

instruction::instruction(uint32_t i)
{
    this->i =  i;
}

std::string instruction::to_string()
{
    std::stringstream ss;
    uint32_t target;

    opcode_i_type opcode(this->i);

    switch(opcode.op)
    {
        case _SPECIAL:
            return this->special_to_string();
            break;
        case _REGIMM:
            return this->regimm_to_string();
            break;
        case JAL:
            target = this->i & 0x3FFFFFF;
            ss << "JAL 0x" << std::hex << upper << target;
            break;
        case BEQ:
            ss << "BEQ " << std::dec << static_cast<unsigned>(opcode.rs) << ", " 
               << static_cast<unsigned>(opcode.rt) << ", 0x" << std::hex << upper << opcode.immediate;
            break;
        case BNE:
            ss << "BNE " << std::dec << static_cast<unsigned>(opcode.rs) << ", "
               << static_cast<unsigned>(opcode.rt) << ", 0x" << std::hex << upper << opcode.immediate;
            break;
        case ADDI:
            ss << "ADDI " << std::dec << static_cast<unsigned>(opcode.rt) << ", "
               << static_cast<unsigned>(opcode.rs) << ", 0x" << std::hex << upper << opcode.immediate;
            break;
        case ADDIU:
            ss << "ADDIU " << std::dec << static_cast<unsigned>(opcode.rt) << ", "
               << static_cast<unsigned>(opcode.rs) << ", 0x" << std::hex << upper << opcode.immediate;
            break;
        case ANDI:
            ss << "ANDI " << std::dec << static_cast<unsigned>(opcode.rt) << ", "
               << static_cast<unsigned>(opcode.rs) << ", 0x" << std::hex << upper << opcode.immediate;
            break;
        case ORI:
            ss << "ORI " << std::dec << static_cast<unsigned>(opcode.rt) << ", "
               << static_cast<unsigned>(opcode.rs) << ", 0x" << std::hex << upper << opcode.immediate;
            break;
        case LUI:
            ss << "LUI " << std::dec << static_cast<unsigned>(opcode.rt) << ", 0x" << std::hex << upper << opcode.immediate;
            break;
        case _CP0:
            return this->cp0_to_string();
            break;
        case _CP1:
            return this->cp1_to_string();
            break;
        case BEQL:
            ss << "BEQL " << std::dec << static_cast<unsigned>(opcode.rs) << ", " 
               << static_cast<unsigned>(opcode.rt) << ", 0x" << std::hex << upper << opcode.immediate;
            break;
        case BNEL:
            ss << "BNEL " << std::dec << static_cast<unsigned>(opcode.rs) << ", "
               << static_cast<unsigned>(opcode.rt) << ", 0x" << std::hex << upper << opcode.immediate;
            break;
        case DADDI:
            ss << "DADDI " << std::dec << static_cast<unsigned>(opcode.rs) << ", "
               << static_cast<unsigned>(opcode.rt) << ", 0x" << std::hex << upper << opcode.immediate;
            break;
        case LW:
            ss << "LW " << std::dec << static_cast<unsigned>(opcode.rt) << ", 0x" << std::hex << upper << opcode.immediate << "(" << std::dec << static_cast<unsigned>(opcode.rs) << ")";
            break;
        case SW:
            ss << "SW " << std::dec << static_cast<unsigned>(opcode.rt) << ", 0x" << std::hex << upper << opcode.immediate << "(" << std::dec << static_cast<unsigned>(opcode.rs) << ")";
            break;
        case SD:
            ss << "SD " << std::dec << static_cast<unsigned>(opcode.rt) << ", 0x" << std::hex << upper << opcode.immediate << "(" << std::dec << static_cast<unsigned>(opcode.rs) << ")";
            break;
        default:
            ss << "Unknown opcode 0x" << std::hex << upper << static_cast<unsigned>(opcode.op);
            break;
    }

    return ss.str();
}

std::string instruction::cp0_to_string()
{
    std::stringstream ss;

    opcode_co_type opcode(this->i);

    switch(opcode.sub)
    {
        case MFC0:
            ss << "MFC0 " << std::dec << static_cast<unsigned>(opcode.rt) << ", " << static_cast<unsigned>(opcode.rd);
            break;
        case MTC0:
            ss << "MTC0 " << std::dec << static_cast<unsigned>(opcode.rt) << ", " << static_cast<unsigned>(opcode.rd);
            break;
        default:
            ss << "Unknown CP0 sub-opcode 0x" << std::hex << upper << static_cast<unsigned>(opcode.sub);
            break;
    }
    return ss.str();
}

std::string instruction::cp1_to_string()
{
    std::stringstream ss;

    opcode_co_type opcode(this->i);

    switch(opcode.sub)
    {
        case MFC1:
            ss << "MFC1 " << std::dec << static_cast<unsigned>(opcode.rt) << ", " << static_cast<unsigned>(opcode.rd);
            break;
        default:
            ss << "Unknown CP1 sub-opcode 0x" << std::hex << upper << static_cast<unsigned>(opcode.sub);
            break;
    }
    return ss.str();
}

std::string instruction::special_to_string()
{
    std::stringstream ss;

    opcode_special_type opcode(this->i);

    switch(opcode.function)
    {
        case SLL:
            ss << "SLL " << std::dec << static_cast<unsigned>(opcode.rd) << ", " << static_cast<unsigned>(opcode.rt) << ", " << static_cast<unsigned>(opcode.sa);
            break;
        case SRL:
            ss << "SRL " << std::dec << static_cast<unsigned>(opcode.rd) << ", " << static_cast<unsigned>(opcode.rt) << ", " << static_cast<unsigned>(opcode.sa);
            break;
        case SLLV:
            ss << "SLLV " << std::dec << static_cast<unsigned>(opcode.rd) << ", " << static_cast<unsigned>(opcode.rt) << ", " << static_cast<unsigned>(opcode.sa);
            break;
        case SRLV:
            ss << "SRLV " << std::dec << static_cast<unsigned>(opcode.rd) << ", " << static_cast<unsigned>(opcode.rt) << ", " << static_cast<unsigned>(opcode.sa);
            break;
        case JR:
            ss << "JR " << std::dec << static_cast<unsigned>(opcode.rs);
            break;
        case MFHI:
            ss << "MFHI " << std::dec << static_cast<unsigned>(opcode.rd);
            break;
        case MFLO:
            ss << "MFLO " << std::dec << static_cast<unsigned>(opcode.rd);
            break;
        case MTLO:
            ss << "MTLO " << std::dec << static_cast<unsigned>(opcode.rd);
            break;
        case MULTU:
            ss << "MULTU " << std::dec << static_cast<unsigned>(opcode.rs) << ", " << static_cast<unsigned>(opcode.rt);
            break;
        case ADDU:
            ss << "ADDU " << std::dec << static_cast<unsigned>(opcode.rd) << ", " << static_cast<unsigned>(opcode.rs) << ", " << static_cast<unsigned>(opcode.rt);
            break;
        case SUBU:
            ss << "SUBU " << std::dec << static_cast<unsigned>(opcode.rd) << ", " << static_cast<unsigned>(opcode.rs) << ", " << static_cast<unsigned>(opcode.rt);
            break;
        case AND:
            ss << "AND " << std::dec << static_cast<unsigned>(opcode.rd) << ", " << static_cast<unsigned>(opcode.rs) << ", " << static_cast<unsigned>(opcode.rt);
            break;
        case OR:
            ss << "OR " << std::dec << static_cast<unsigned>(opcode.rd) << ", " << static_cast<unsigned>(opcode.rs) << ", " << static_cast<unsigned>(opcode.rt);
            break;
        case XOR:
            ss << "XOR " << std::dec << static_cast<unsigned>(opcode.rd) << ", " << static_cast<unsigned>(opcode.rs) << ", " << static_cast<unsigned>(opcode.rt);
            break;
        case SLTU:
            ss << "SLTU " << std::dec << static_cast<unsigned>(opcode.rd) << ", " << static_cast<unsigned>(opcode.rs) << ", " << static_cast<unsigned>(opcode.rt);
            break;
        case DSLL32:
            ss << "DSLL32 " << std::dec << static_cast<unsigned>(opcode.rd) << ", " << static_cast<unsigned>(opcode.rt) << ", " << static_cast<unsigned>(opcode.sa);
            break;
        default:
            ss << "Unknown SPECIAL sub-opcode 0x" << std::hex << upper << static_cast<unsigned>(opcode.function);
            break;
    }
    return ss.str();
}

std::string instruction::regimm_to_string()
{
    std::stringstream ss;

    opcode_regimm_type opcode(this->i);

    switch(opcode.rt)
    {
        case BLTZ:
            ss << "BLTZ " << std::dec << static_cast<unsigned>(opcode.rs) << ", " << std::hex << upper << static_cast<unsigned>(opcode.offset);
            break;
        case BGEZAL:
            ss << "BGEZAL " << std::dec << static_cast<unsigned>(opcode.rs) << ", " << std::hex << upper << static_cast<unsigned>(opcode.offset);
            break;
        default:
            ss << "Unknown REGIMM sub-opcode 0x" << std::hex << upper << static_cast<unsigned>(opcode.rt);
            break;
    }
    return ss.str();
}
