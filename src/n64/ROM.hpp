#pragma once

#include <string>
#include "N64.hpp"
#include "CRC.h"

const uint32_t ROM_LOWHIGH = 0x80371240;
const uint32_t ROM_HIGHLOW = 0x12408037;

typedef struct ROM_header
{
    uint32_t signature;
    uint32_t clockrate;
    uint32_t pc;
    uint32_t release;
    uint32_t crc1;
    uint32_t crc2;
    uint64_t reserved_1;
    char name[20];
    uint32_t reserved_2;
    uint32_t manufacturer_id;
    uint16_t cartridge_id;
    uint16_t country_code;
} __attribute__((__packed__)) ROM_header;

class ROM
{
  public:
    ROM();
    const ROM_header *header;
    const size_t size();
    const std::byte *get_pointer();
    void open(std::string filename);
    const uint32_t crc();
  private:
    std::byte *data = nullptr;
    std::string filename;
    size_t filesize = 0;
    void byte_swap();
};
