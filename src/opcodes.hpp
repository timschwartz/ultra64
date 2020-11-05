#pragma once

#include <string>
#include <iostream>

namespace ultra64
{
    class opcode_i_type
    {
      public:
        uint8_t op;
        uint8_t rs;
        uint8_t rt;
        uint16_t immediate;
        opcode_i_type(uint32_t instruction)
        {
            this->op = instruction >> 26;
            this->rs = (instruction >> 21) & 0x1F;
            this->rt = (instruction >> 16) & 0x1F;
            this->immediate = instruction & 0xFFFF;
        }
    };

    class opcode_co_type
    {
      public:
        uint8_t op;
        uint8_t sub;
        uint8_t rt;
        uint8_t rd;
        opcode_co_type(uint32_t instruction)
        {
            this->op = instruction >> 26;
            this->sub = (instruction >> 21) & 0x1F;
            this->rt = (instruction >> 16) & 0x1F;
            this->rt = (instruction >> 11) & 0x1F;
        }
    };

    const uint8_t _SPECIAL = 0x00;
    const uint8_t _REGIMM = 0x01;
    const uint8_t ADDIU = 0x09;
    const uint8_t ANDI = 0x0C;
    const uint8_t ORI = 0x0D;
    const uint8_t LUI = 0x0F;
    const uint8_t _CP0 = 0x10;
    const uint8_t BEQL = 0x14;
    const uint8_t BNEL = 0x15;
    const uint8_t LW = 0x23;
    const uint8_t SW = 0x2B;

    // CP0
    const uint8_t MTC0 = 0x04;
}
