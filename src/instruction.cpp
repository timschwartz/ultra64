#include "instruction.hpp"
#include "opcodes.hpp"

#include <sstream>
#include <iostream>

using namespace ultra64;

instruction::instruction(uint32_t i)
{
    this->i =  i;
}

std::string instruction::to_string()
{
    std::stringstream ss;
//    ss << "0x" << std::hex << this->i << " ";

    opcode_i_type opcode(this->i);

    switch(opcode.op)
    {
        case ANDI:
            ss << "ANDI " << std::dec << static_cast<unsigned>(opcode.rt) << ", " << static_cast<unsigned>(opcode.rs) << ", 0x" << std::hex << opcode.immediate;
            break;
        case ORI:
            ss << "ORI " << std::dec << static_cast<unsigned>(opcode.rt) << ", " << static_cast<unsigned>(opcode.rs) << ", 0x" << std::hex << opcode.immediate;
            break;
        case LUI:
            ss << "LUI " << std::dec << static_cast<unsigned>(opcode.rt) << ", 0x" << std::hex << opcode.immediate;
            break;
        case _CP0:
            return this->cp0_to_string();
            break;
        case BEQL:
            ss << "BEQL " << std::dec << static_cast<unsigned>(opcode.rs) << ", " << static_cast<unsigned>(opcode.rt) << ", 0x" << std::hex << opcode.immediate;
            break;
        case LW:
            ss << "LW " << std::dec << static_cast<unsigned>(opcode.rt) << ", 0x" << std::hex << opcode.immediate << "(" << static_cast<unsigned>(opcode.rs) << ")";
            break;
        default:
            ss << "Unknown opcode 0x" << std::hex << static_cast<unsigned>(opcode.op);
            break;
    }

    return ss.str();
}

std::string instruction::cp0_to_string()
{
    std::stringstream ss;
//    ss << "0x" << std::hex << this->i << " ";

    opcode_co_type opcode(this->i);

    switch(opcode.sub)
    {
        case MTC0:
            ss << "MTC0 " << std::dec << static_cast<unsigned>(opcode.rt) << ", " << static_cast<unsigned>(opcode.rd);
            break;
        default:
            ss << "Unknown CP0 sub-opcode 0x" << std::hex << static_cast<unsigned>(opcode.sub);
            break;
    }
    return ss.str();
}
