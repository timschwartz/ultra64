#include <fstream>
#include "PIF.hpp" 
#include <iostream>

namespace ultra64
{
    PIF::PIF(std::string filename)
    {
        std::fstream file;
        file.open(filename.c_str(), std::fstream::in);

        if(!file.good())
        {
            throw std::runtime_error("Couldn't open file " +  filename);
        }

        file.seekg(0, std::ios::end);
        this->filesize = file.tellg();

        this->data = new std::byte[this->filesize];
        file.seekg(0, std::ios::beg);
        file.read((char *)this->data, this->filesize);
        file.close();

        for(size_t count = 0; count < this->filesize; count += 4)
        {
            uint8_t *ptr = (uint8_t *)(this->data + count);
            uint32_t dword = *(uint32_t *)ptr;

            ptr[0] = (dword & 0xFF000000) >> 24;
            ptr[1] = (dword & 0x00FF0000) >> 16;
            ptr[2] = (dword & 0x0000FF00) >> 8;
            ptr[3] = dword & 0x000000FF;
        }
    }

    std::byte *PIF::get_pointer()
    {
        return data;
    }

    PIF::~PIF()
    {
        delete data;
    }
}
