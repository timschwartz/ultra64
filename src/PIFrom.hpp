#pragma once

#include <string>

namespace ultra64 
{
    class PIFrom
    {
      public:
        PIFrom(std::string filename);
        ~PIFrom();
        std::byte *get_pointer();
      private:
        size_t filesize;
        std::byte *data;
    };
}

