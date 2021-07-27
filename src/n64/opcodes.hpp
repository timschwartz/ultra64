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

    class opcode_special_type
    {
      public:
        uint8_t op;
        uint8_t rs;
        uint8_t rt;
        uint8_t rd;
        uint8_t sa;
        uint8_t function;
        opcode_special_type(uint32_t instruction)
        {
            this->op = instruction >> 26;
            this->rs = (instruction >> 21) & 0x1F;
            this->rt = (instruction >> 16) & 0x1F;
            this->rd = (instruction >> 11) & 0x1F;
            this->sa = (instruction >> 6) & 0x1F;
            this->function = instruction & 0x3F;
        }
    };

    class opcode_regimm_type
    {
      public:
        uint8_t op;
        uint8_t rs;
        uint8_t rt;
        uint16_t offset;
        opcode_regimm_type(uint32_t instruction)
        {
            this->op = instruction >> 26;
            this->rs = (instruction >> 21) & 0x1F;
            this->rt = (instruction >> 16) & 0x1F;
            this->offset = instruction & 0xFFFF;
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
            this->rd = (instruction >> 11) & 0x1F;
        }
    };

    const uint8_t _SPECIAL = 0x00;
    const uint8_t _REGIMM = 0x01;
    const uint8_t JAL = 0x03;
    const uint8_t BEQ = 0x04;
    const uint8_t BNE = 0x05;
    const uint8_t ADDI = 0x08;
    const uint8_t ADDIU = 0x09;
    const uint8_t ANDI = 0x0C;
    const uint8_t ORI = 0x0D;
    const uint8_t LUI = 0x0F;
    const uint8_t _CP0 = 0x10;
    const uint8_t _CP1 = 0x11;
    const uint8_t BEQL = 0x14;
    const uint8_t BNEL = 0x15;
    const uint8_t DADDI = 0x18;
    const uint8_t LW = 0x23;
    const uint8_t SW = 0x2B;
    const uint8_t SD = 0x3F;

    // SPECIAL
    const uint8_t SLL = 0x00;
    const uint8_t SRL = 0x02;
    const uint8_t SLLV = 0x04;
    const uint8_t SRLV = 0x06;
    const uint8_t JR = 0x08;
    const uint8_t MFHI = 0x10;
    const uint8_t MFLO = 0x12;
    const uint8_t MTLO = 0x13;
    const uint8_t MULTU = 0x19;
    const uint8_t ADDU = 0x21;
    const uint8_t SUBU = 0x23;
    const uint8_t AND = 0x24;
    const uint8_t OR = 0x25;
    const uint8_t XOR = 0x26;
    const uint8_t SLTU = 0x2B;
    const uint8_t DSLL32 = 0x3C;

    // REGIMM
    const uint8_t BLTZ = 0x00;
    const uint8_t BGEZAL = 0x11;

    // CP0
    const uint8_t MFC0 = 0x00;
    const uint8_t MTC0 = 0x04;

    // CP1
    const uint8_t MFC1 = 0x00; 
}
