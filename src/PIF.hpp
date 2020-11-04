#pragma once

#include <string>

namespace ultra64 
{
    class PIF
    {
      public:
        PIF(std::string filename);
        ~PIF();
        std::byte *get_pointer();
      private:
        size_t filesize;
        std::byte *data;
    };
}

