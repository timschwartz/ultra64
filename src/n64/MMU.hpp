#pragma once

#include <string>
#include <unordered_map>
#include <functional>

namespace ultra64
{
    class MMU;
    struct memory_section;

    typedef struct memory_section
    {
        uint32_t addr = 0;
        uint32_t offset = 0;
        uint32_t size = 0;
        uint32_t max_size = 0;
        std::byte *ptr = nullptr;
        void (*write_handler)(MMU *mmu, memory_section s, uint64_t value) = nullptr;
        void (*read_handler)(MMU *mmu, memory_section s, uint64_t value) = nullptr;
    } memory_section;

    class MMU
    {
      public:
        MMU();
        ~MMU();
        void register_memory(std::string name, memory_section s);
        void write_word(uint32_t addr, uint32_t value);
        void write_word_raw(uint32_t addr, uint32_t value);
        void write_word_impl(uint32_t addr, uint32_t value, bool call_handler);
        uint32_t read_word(uint32_t addr);
        uint32_t read_word_raw(uint32_t addr);
        uint32_t read_word_impl(uint32_t addr, bool call_handler);
        void Dump();
        memory_section get_section(uint32_t addr);
        std::unordered_map<std::string, memory_section> memory;
    };

    void map_memory(MMU *mmu, std::string name, uint32_t addr, uint32_t size, uint32_t max_size, std::byte *ptr,
                void (*write_handler)(MMU *mmu, memory_section s, uint64_t value),
                void (*read_handler)(MMU *mmu, memory_section s, uint64_t value));
}

auto const SI_STATUS_REG = 0x04800018;
