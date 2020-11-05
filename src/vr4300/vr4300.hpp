#pragma once

#include "../MMU.hpp"
#include "opcodes.hpp"
#include <cstdint>

#define IMPL_OP(x) static void x(vr4300 *);

namespace ultra64
{
    class vr4300
    {
      public:
        vr4300(MMU &mmu);
        uint32_t get_PC();
        IMPL_OP(not_implemented);
        IMPL_OP(addiu);
        IMPL_OP(andi);
        IMPL_OP(ori);
        IMPL_OP(lui);
        IMPL_OP(cp0);
        IMPL_OP(beql);
        IMPL_OP(lw);
        // CP0
        IMPL_OP(mtc0);
        void step();
      private:
        uint32_t PC;
        uint64_t GPR[32] = { 0 };
        uint64_t CP0[32] = { 0 };
        void (*opcode[0x40])(vr4300 *);
        void (*opcode_cp0[0x40])(vr4300 *);
        uint32_t current_instruction;
        MMU *mmu;
    };
}
