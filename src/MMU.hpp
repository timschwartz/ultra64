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
        void (*write_handler)(MMU *mmu, memory_section s) = nullptr;
        void (*read_handler)(MMU *mmu, memory_section s, uint64_t value) = nullptr;
    } memory_section;

    class MMU
    {
      public:
        MMU();
        ~MMU();
        void register_memory(std::string name, memory_section s);
        void write_word(uint32_t addr, uint32_t value);
        uint32_t read_word(uint32_t addr);
      private:
        memory_section get_section(uint32_t addr);
        std::unordered_map<std::string, memory_section> memory;
    };
}
