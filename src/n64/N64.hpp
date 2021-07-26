#pragma once

#include "MMU.hpp"
#include "vr4300/vr4300.hpp"
#include "rcp/RSP.hpp"
#include "ROM.hpp"

namespace ultra64 
{
    class ROM;
    class N64
    {
        public:
          N64()
          {
              this->mmu = new MMU();
              this->cpu.set_n64(this);
              this->rsp.set_n64(this);
          };
          MMU *mmu;
          vr4300 cpu;
          RSP rsp;
          ROM *rom;
    };
}
