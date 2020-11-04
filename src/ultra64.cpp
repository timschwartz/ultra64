#include "MMU.hpp"
#include "ROM.hpp"
#include "PIF.hpp"
#include "vr4300/vr4300.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    ultra64::PIF pif("pifdata.bin");

    uint32_t count = 0;
    uint32_t *p = (uint32_t *)pif.get_pointer();

    ultra64::MMU mmu;

    while(count < 0x7C0)
    {
        mmu.write_word(0x1FC00000 + count, p[count / 4]);
        count += 4;
    }

    std::string filename;
    if(argc == 2)
    {
        filename = std::string(argv[1]);
    }

    if(filename.size())
    {
        ultra64::ROM rom(filename);
        ultra64::memory_section s;
        s.offset = 0x10000000;
        s.max_size = 0x0FBFFFFF;
        s.size = rom.size();
        s.ptr = rom.get_pointer();
        mmu.register_memory("rom", s);
    }

    ultra64::vr4300 cpu(mmu);

    while(1) cpu.step();
    
    return 0;
}
