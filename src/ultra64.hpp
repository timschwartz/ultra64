#pragma once

#include "json.hpp"

class ultra64
{
  protected:
    std::string config_path = ".config/ultra64";
    nlohmann::json config;
  public:
    void load_config();
    void save_config();
};
