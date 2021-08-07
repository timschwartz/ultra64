#include "MMU.hpp"
#include "rcp/RSP.hpp"
#include <iostream>
#include <sstream>
#include <cstring>

using namespace ultra64;

void sp_register_write(MMU *mmu, memory_section s, uint64_t value)
{
    switch(s.addr)
    {
        case SP_STATUS_REG:
            std::cout << "  Writing to SP_STATUS_REG:" << std::endl;
            if(value & 0x01) std::cout << "    - Clear halt" << std::endl;
            if(value & 0x02) std::cout << "    - Set halt" << std::endl;
            if(value & 0x04) std::cout << "    - Clear broke" << std::endl;
            if(value & 0x08) std::cout << "    - Clear interrupt" << std::endl;
            break;
        default:
            std::cout << "    sp_register_write(): Wrote " << std::hex << value << " to " << s.addr << std::endl;
            break;
    }
}

void sp_register_read(MMU *mmu, memory_section s, uint64_t value)
{
    switch(s.addr)
    {
        case SP_STATUS_REG:
            std::cout << "  Reading from SP_STATUS_REG:" << std::endl;
            if(value & 0x01) std::cout << "    - Halt" << std::endl;
            if(value & 0x02) std::cout << "    - Broke" << std::endl;
            if(value & 0x04) std::cout << "    - DMA busy" << std::endl;
            if(value & 0x08) std::cout << "    - DMA full" << std::endl;
            break;
        case SP_DMA_BUSY_REG:
            std::cout << "  Reading from SP_DMA_BUSY_REG:" << std::endl;
            if(value & 0x01) std::cout << "    - Busy" << std::endl;
            break;
        default:
            std::cout << "    sp_register_read(): Read " << std::hex << value << " from " << s.addr << std::endl;
            break;
    }
}

void pi_register_write(MMU *mmu, memory_section s, uint64_t value)
{
    std::cout << "    pi_register_write(): Wrote " << std::hex << value << " to " << s.addr << std::endl;
}

void si_register_write(MMU *mmu, memory_section s, uint64_t value)
{
    switch(s.addr)
    {
        case SI_STATUS_REG:
            std::cout << "  Writing to SI_STATUS_REG:" << std::endl;
            if(value & 0x01) std::cout << "    - DMA busy" << std::endl;
            if(value & 0x02) std::cout << "    - IO read busy" << std::endl;
            if(value & 0x04) std::cout << "    - Reserved" << std::endl;
            if(value & 0x08) std::cout << "    - DMA error" << std::endl;
            if(value & 0x0800) std::cout << "    - Interrupt" << std::endl;
            break;
        default:
            std::cout << "    si_register_write(): Wrote " << std::hex << value << " to " << s.addr << std::endl;
            break;
    }
}

void si_register_read(MMU *mmu, memory_section s, uint64_t value)
{
    switch(s.addr)
    {
        case SI_STATUS_REG:
            std::cout << "  Reading from SI_STATUS_REG:" << std::endl;
            if(value & 0x01) std::cout << "    - DMA busy" << std::endl;
            if(value & 0x02) std::cout << "    - IO read busy" << std::endl;
            if(value & 0x04) std::cout << "    - Reserved" << std::endl;
            if(value & 0x08) std::cout << "    - DMA error" << std::endl;
            if(value & 0x0800) std::cout << "    - Interrupt" << std::endl;
            break;
        default:
            std::cout << "    si_register_read(): Read " << std::hex << value << " from " << s.addr << std::endl;
            break;
    }
}

void pif_ram_write(MMU *mmu, memory_section s, uint64_t value)
{
    std::cout << "    pif_ram_write(): Wrote " << std::hex << value << " to " << s.addr << std::endl;
}

void dp_register_write(MMU *mmu, memory_section s, uint64_t value)
{
    std::cout << "    dp_register_write(): Wrote " << std::hex << value << " to " << s.addr << std::endl;
}

void dp_register_read(MMU *mmu, memory_section s, uint64_t value)
{
    std::cout << "    dp_register_read(): Read " << std::hex << value << " from " << s.addr << std::endl;
}

void sp_dmem_write(MMU *mmu, memory_section s, uint64_t value)
{
    std::cout << "    sp_dmem_write(): Wrote " << std::hex << value << " to " << s.addr << std::endl;
}

void ultra64::map_memory(MMU *mmu, std::string name, uint32_t addr, uint32_t size, uint32_t max_size, std::byte *ptr, 
                void (*write_handler)(MMU *mmu, memory_section s, uint64_t value),
                void (*read_handler)(MMU *mmu, memory_section s, uint64_t value))
{
    memory_section s;

    s.offset = addr;
    s.size = size;
    s.max_size = max_size;
    s.ptr = ptr;
    s.write_handler = write_handler;
    s.read_handler = read_handler;
    memset(s.ptr, 0, s.size);

    mmu->register_memory(name, s);
}

MMU::MMU()
{
    map_memory(this, "pif_ram", 0x1FC007C0, 0x40, 0x40, new std::byte[0x40], &pif_ram_write, nullptr);
    map_memory(this, "sp_dmem", 0x04000000, 0x1000, 0x1000, new std::byte[0x1000], &sp_dmem_write, nullptr);
    map_memory(this, "sp_imem", 0x04001000, 0x1000, 0x1000, new std::byte[0x1000], nullptr, nullptr);
    map_memory(this, "sp_registers", 0x04040000, 0x20, 0x20, new std::byte[0x20], &sp_register_write, &sp_register_read);
    map_memory(this, "dp_registers", 0x04100000, 0x20, 0x20, new std::byte[0x20], &dp_register_write, &dp_register_read);
    map_memory(this, "vi_registers", 0x04400000, 0x38, 0x38, new std::byte[0x38], nullptr, nullptr);
    map_memory(this, "ai_registers", 0x04500000, 0x18, 0x18, new std::byte[0x18], nullptr, nullptr);
    map_memory(this, "pi_registers", 0x04600000, 0x34, 0x34, new std::byte[0x34], &pi_register_write, nullptr);
    map_memory(this, "si_registers", 0x04800000, 0x1C, 0x1C, new std::byte[0x1C], nullptr, &si_register_read);
    map_memory(this, "dd_ipl_rom", 0x06000000, 0x400000, 0x400000, new std::byte[0x400000], nullptr, nullptr);
}

void MMU::Dump()
{
    std::cout << std::dec << this->memory.size() << " memory areas" << std::endl;
    for(const auto &[name, section] : this->memory)
    {
        std::cout << name << ": " << std::dec << section.size << " bytes at " << std::hex << section.offset << std::endl;
    }
}

MMU::~MMU()
{
    return;
    for(const auto &it : this->memory)
    {
        if(it.first.size() == 0) continue;
        std::cout << "Deleting " << it.first << std::endl;
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
    this->write_word_impl(addr, value, true);
}

void MMU::write_word_raw(uint32_t addr, uint32_t value)
{
    this->write_word_impl(addr, value, false);
}

void MMU::write_word_impl(uint32_t addr, uint32_t value, bool call_handler)
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

    uint32_t offset = phys_addr - s.offset;
    uint32_t *p = (uint32_t *)(s.ptr + offset);

    *p = value;
    if(!call_handler) return;
    if(s.write_handler != nullptr) s.write_handler(this, s, value);
}

uint32_t MMU::read_word(uint32_t addr)
{
    return this->read_word_impl(addr, true);
}

uint32_t MMU::read_word_raw(uint32_t addr)
{
    return this->read_word_impl(addr, false);
}

uint32_t MMU::read_word_impl(uint32_t addr, bool call_handler)
{
    uint32_t phys_addr = addr & 0x1FFFFFFF;
    memory_section s = get_section(phys_addr);

    uint32_t pointer = phys_addr - s.offset;

    uint32_t value = *(uint32_t *)(s.ptr + pointer);

    if(!call_handler) return value;

    if(s.read_handler != nullptr) s.read_handler(this, s, value);
    return value;
}
