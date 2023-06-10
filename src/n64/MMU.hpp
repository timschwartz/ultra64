#pragma once

#include <string>
#include <unordered_map>
#include <functional>

typedef struct memory_section
{
    uint32_t addr = 0;
    uint32_t offset = 0;
    uint32_t size = 0;
    uint32_t max_size = 0;
    std::byte *ptr = nullptr;
    void (*write_handler)(memory_section s, uint64_t value) = nullptr;
    void (*read_handler)(memory_section s, uint64_t value) = nullptr;
} memory_section;

class MMU
{
  public:
    MMU();
    ~MMU();
    static void generic_write_logger(memory_section s, uint64_t value);
    static void generic_read_logger(memory_section s, uint64_t value);
    void memory_register(std::string name, memory_section s);
    void memory_clear();
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

auto const SI_STATUS_REG = 0x04800018;
