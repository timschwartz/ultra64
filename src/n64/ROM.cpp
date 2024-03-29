#include "ROM.hpp"
#include <fstream>

#include <iostream>

using namespace ultra64;

void rom_write(MMU *mmu, memory_section s, uint64_t value)
{
    std::cout << "    ROM: Wrote " << std::hex << value << " to " << s.addr << std::endl;
}

void rom_read(MMU *mmu, memory_section s, uint64_t value)
{
    std::cout << "    ROM: Read " << std::hex << value << " from " << s.addr << std::endl;
}

ROM::ROM()
{
}

void ROM::Open(N64 *n64, std::string filename)
{
    this->filename = filename;

    std::fstream file;
    file.open(filename.c_str(), std::fstream::in);

    if(!file.good())
    {
        throw std::runtime_error("Couldn't open file " +  filename);
    }

    std::cout << "Reading " << filename << std::endl;

    file.seekg(0, std::ios::end);
    this->filesize = file.tellg();
    this->data = new std::byte[this->filesize];
    file.seekg(0, std::ios::beg);
    file.read((char *)this->data, this->filesize);
    file.close();

    std::cout << "File format: ";

    this->header = (ROM_header *)this->data;
    switch(header->signature)
    {
        case ROM_HIGHLOW: 
            std::cout << "Byte-swapped" << std::endl;
            this->byte_swap();
            break;
        case ROM_LOWHIGH:
            std::cout << "Not byte-swapped" << std::endl;
            break;
        default:
            std::cout << "Unknown format - 0x" << std::hex << header->signature << std::endl;
            break;
    }

    std::cout << "Game name: " << this->header->name << std::endl;
    std::cout << "PC: 0x" << std::hex << this->header->pc << std::endl;

    ultra64::map_memory(n64->mmu, "rom", 0x10000000, this->filesize, 0x0FBFFFFF, new std::byte[this->filesize], nullptr, nullptr);
    
    uint32_t count = 0;
    uint32_t *v = (uint32_t *)this->data;
    while(count < this->filesize)
    {
        n64->mmu->write_word_raw(0x10000000 + count, *v);
        v++;
        count += 4;
    }    

    for(auto &[name, section] : n64->mmu->memory)
    {
        if(name != "rom") continue;
        section.write_handler = &rom_write;
        section.read_handler = &rom_read;
    }
}

void ROM::byte_swap()
{
    for(size_t count = 0; count < this->filesize; count += 2)
    {
        uint8_t *ptr = (uint8_t *)(this->data + count);
        uint16_t hword = *(uint16_t *)ptr;

        ptr[0] = (hword & 0xFF00) >> 8;
        ptr[1] = hword & 0x00FF;
    }
}

const size_t ROM::size()
{
    return this->filesize;
}

std::byte *ROM::get_pointer()
{
    return this->data;
}
