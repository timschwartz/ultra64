#pragma once

#include <string>

class instruction
{
  public:
    instruction(uint32_t);
    std::string to_string();
    std::string cp0_to_string();
    std::string cp1_to_string();
    std::string special_to_string();
    std::string regimm_to_string();
  private:
    uint32_t i;
};
