#pragma once

#include <string>

namespace ultra64
{
    class instruction
    {
      public:
        instruction(uint32_t);
        std::string to_string();
        std::string cp0_to_string();
      private:
        uint32_t i;
    };
}
