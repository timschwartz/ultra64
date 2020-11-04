#pragma once

#include <string>

const uint32_t ROM_LOWHIGH = 0x80371240;
const uint32_t ROM_HIGHLOW = 0x12408037;

namespace ultra64
{
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
        ROM(std::string filename);
        const ROM_header *header;
        const size_t size();
        std::byte *get_pointer();
      private:
        std::string filename;
        std::byte *data = nullptr;
        size_t filesize = 0;
        void byte_swap();
    };
}
