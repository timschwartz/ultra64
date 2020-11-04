#include "MMU.hpp"
#include <iostream>
#include <sstream>

using namespace ultra64;

MMU::MMU()
{
    memory_section s;

    s.offset = 0x1FC00000;
    s.size = 0x7C0;
    s.max_size = s.size;
    s.ptr = new std::byte[0x7C0];
    this->register_memory("pif_rom", s);

    s.offset = 0x04040000;
    s.size = 0x1F;
    s.max_size = s.size;
    s.ptr = new std::byte[0x1F];
    this->register_memory("sp_registers", s);
}

MMU::~MMU()
{
    for(const auto &it : this->memory)
    {
        memory_section s = it.second;
        delete s.ptr;
        this->memory.erase(it.first);
    }
}

void MMU::register_memory(std::string name, memory_section s)
{
    this->memory[name] = s;
}

memory_section MMU::get_section(uint32_t addr)
{
    for(const auto &it : this->memory)
    {
        memory_section s = it.second;
        if(addr >= s.offset && addr <= (s.offset + s.max_size))
        {
            if(addr - s.offset > s.size)
            {
                std::stringstream ss;
                ss << it.first << " address " << std::hex << addr << " out of bounds.";
                throw std::runtime_error(ss.str());
            }
            s.addr = addr;
            return s;
        }
    }

    std::stringstream ss;
    ss << "Unknown memory address: " << std::hex << addr;
    throw std::runtime_error(ss.str());
}

void MMU::write_word(uint32_t addr, uint32_t value)
{
    uint32_t phys_addr = addr & 0x1FFFFFFF;
    memory_section s = get_section(phys_addr);

    uint32_t pointer = phys_addr - s.offset;
    if(pointer) pointer /= 4;

    uint32_t *p = (uint32_t *)s.ptr;
    asm ("movl %1, %%eax;"
          "bswap %%eax;"
          "movl %%eax, %0;"
         :"=r"(p[pointer])        /* output */
         :"r"(value)         /* input */
         :"%eax"         /* clobbered register */
     );
}

uint32_t MMU::read_word(uint32_t addr)
{
    uint32_t phys_addr = addr & 0x1FFFFFFF;
    memory_section s = get_section(phys_addr);

    uint32_t pointer = phys_addr - s.offset;
    if(pointer) pointer /= 4;

    uint32_t *p = (uint32_t *)s.ptr;
    uint32_t value;

    asm ("movl %1, %%eax;" 
          "bswap %%eax;"
          "movl %%eax, %0;"
         :"=r"(value)        /* output */
         :"r"(p[pointer])         /* input */
         :"%eax"         /* clobbered register */
     );

    return value;
}
