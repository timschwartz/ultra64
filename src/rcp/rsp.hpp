#pragma once

#include "../MMU.hpp"

/** RSP status register */
const uint32_t SP_STATUS_REG = 0x04040010;

/** RSP Halt */
const uint32_t  SP_STATUS_HALT = 0x00000001;
/** RSP broke */
const uint32_t  SP_STATUS_BROKE	= 0x00000002;

namespace ultra64
{
    class rsp
    {
      public:
        rsp(MMU &mmu);
        ~rsp();
        void step();
      private:
       MMU *mmu = nullptr;
    };
}
