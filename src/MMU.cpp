#include "MMU.hpp"
#include <iostream>
#include <sstream>
#include <cstring>

using namespace ultra64;

void sp_register_write(MMU *mmu, memory_section s)
{
    std::cout << "sp_register_write(): Wrote " << std::hex << mmu->read_word(s.addr) << " to " << s.addr << std::endl;
}

void pi_register_write(MMU *mmu, memory_section s)
{
    std::cout << "pi_register_write(): Wrote " << std::hex << mmu->read_word(s.addr) << " to " << s.addr << std::endl;
}

void si_register_read(MMU *mmu, memory_section s, uint64_t value)
{
    std::cout << "si_register_read(): Read " << std::hex << value << " from " << s.addr << std::endl;
}

MMU::MMU()
{
    memory_section s;

#if 0
    s.offset = 0x1FC00000;
    s.size = 0x7C0;
    s.max_size = s.size;
    s.ptr = new std::byte[s.size];
    this->register_memory("pif_rom", s);
#endif

    s.offset = 0x1FC007C0;
    s.size = 0x40;
    s.max_size = s.size;
    s.ptr = new std::byte[s.size];
    memset(s.ptr, 0, s.size);
    this->register_memory("pif_ram", s);

    s.offset = 0x04000000;
    s.size = 0x1000;
    s.max_size = s.size;
    s.ptr = new std::byte[s.size];
    memset(s.ptr, 0, s.size);
    this->register_memory("sp_dmem", s);

    s.offset = 0x04001000;
    s.size = 0x1000;
    s.max_size = s.size;
    s.ptr = new std::byte[s.size];
    memset(s.ptr, 0, s.size);
    this->register_memory("sp_imem", s);

    s.offset = 0x04040000;
    s.size = 0x20;
    s.max_size = s.size;
    s.ptr = new std::byte[s.size];
    memset(s.ptr, 0, s.size);
    s.write_handler = &sp_register_write;
    this->register_memory("sp_registers", s);
    s.write_handler = nullptr;

    s.offset = 0x04100000;
    s.size = 0x20;
    s.max_size = s.size;
    s.ptr = new std::byte[s.size];
    memset(s.ptr, 0, s.size);
    this->register_memory("dp_registers", s);

    s.offset = 0x04400000;
    s.size = 0x38;
    s.max_size = s.size;
    s.ptr = new std::byte[s.size];
    memset(s.ptr, 0, s.size);
    /* probably need a write handler */
    this->register_memory("vi_registers", s);

    s.offset = 0x04500000;
    s.size = 0x18;
    s.max_size = s.size;
    s.ptr = new std::byte[s.size];
    memset(s.ptr, 0, s.size);
    /* probably need a write handler */
    this->register_memory("ai_registers", s);

    s.offset = 0x04600000;
    s.size = 0x34;
    s.max_size = s.size;
    s.ptr = new std::byte[s.size];
    memset(s.ptr, 0, s.size);
    s.write_handler = &pi_register_write;
    this->register_memory("pi_registers", s);
    s.write_handler = nullptr;

    s.offset = 0x04800000;
    s.size = 0x1C;
    s.max_size = s.size;
    s.ptr = new std::byte[s.size];
    memset(s.ptr, 0, s.size);
    s.read_handler = &si_register_read;
    this->register_memory("si_registers", s);
    s.read_handler = nullptr;

    s.offset = 0x06000000;
    s.size = 0x400000;
    s.max_size = s.size;
    s.ptr = new std::byte[s.size];
    memset(s.ptr, 0, s.size);
    this->register_memory("dd_ipl_rom", s);
}

MMU::~MMU()
{
    return;
    for(const auto &it : this->memory)
    {
        if(it.first.size() == 0) continue;
        std::cout << "Deleting " << it.first << std::endl;
        //memory_section s = it.second;
//        delete s.ptr;
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
        if(addr >= s.offset && addr < (s.offset + s.max_size))
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
    memory_section s;
    try {
        s = get_section(phys_addr);
    } catch (std::runtime_error &e) {
        std::stringstream ss;
        ss << "Failed to write " << std::hex << value << ". " << e.what();
        throw std::runtime_error(ss.str());
    }

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

     if(s.write_handler != nullptr) s.write_handler(this, s);
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

    if(s.read_handler != nullptr) s.read_handler(this, s, value);
    return value;
}
