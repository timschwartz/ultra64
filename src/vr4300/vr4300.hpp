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
        IMPL_OP(special);
        IMPL_OP(regimm);
        IMPL_OP(beq);
        IMPL_OP(bne);
        IMPL_OP(addi);
        IMPL_OP(addiu);
        IMPL_OP(andi);
        IMPL_OP(ori);
        IMPL_OP(lui);
        IMPL_OP(cp0);
        IMPL_OP(beql);
        IMPL_OP(bnel);
        IMPL_OP(daddi);
        IMPL_OP(lw);
        IMPL_OP(sw);
        // SPECIAL
        IMPL_OP(sll);
        IMPL_OP(srl);
        IMPL_OP(jr);
        IMPL_OP(mflo);
        IMPL_OP(multu);
        IMPL_OP(_or);
        IMPL_OP(sltu);
        IMPL_OP(dsll32);
        // REGIMM
        IMPL_OP(bltz);
        IMPL_OP(bgezal);
        // CP0
        IMPL_OP(mtc0);
        void step();
      private:
        uint32_t PC, HI, LO;
        uint64_t GPR[32] = { 0 };
        uint64_t CP0[32] = { 0 };
        void (*opcode[0x40])(vr4300 *);
        void (*opcode_cp0[0x40])(vr4300 *);
        void (*opcode_special[0x40])(vr4300 *);
        void (*opcode_regimm[0x40])(vr4300 *);
        uint32_t current_instruction;
        MMU *mmu;
    };
}
