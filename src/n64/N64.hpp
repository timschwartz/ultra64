#pragma once

#include "vr4300/vr4300.hpp"
#include "rcp/RSP.hpp"

class MMU;
class ROM;
class RSP;

class N64
{
  public:
    N64()
    {
        this->cpu.set_n64(this);
        this->rsp.set_n64(this);
    };
    MMU mmu;
    vr4300 cpu;
    RSP rsp;
    void pif_rom_load(std::string filepath);
};
