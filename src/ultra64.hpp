#pragma once

#include "json.hpp"
#include "n64/N64.hpp"
#include "n64/ROM.hpp"

const uint32_t cic_seed_nus_6102 = 0x00003F3F;
const uint32_t crc_nus_6102 = 0x90BB6CB5;

class ultra64
{
  protected:
    std::string config_path = ".config/ultra64";
    nlohmann::json config;
    N64 n64;
    ROM rom;
  public:
    ultra64();
    std::vector<std::string> validOptions = {
        "pif-rom",
        "log-file"
    };
    std::vector<std::string> hasArgs = {
        "pif-rom",
        "log-file"
    };
    std::map<uint32_t, uint32_t> cicSeeds;
    std::string config_load();
    void config_save();
    std::string config_get();
    void option_set(std::string opt, std::string value);
    bool is_valid_option(std::string opt);
    bool has_args(std::string opt);

    void n64_reset();
    void map_memory(std::string name, uint32_t addr, uint32_t size, uint32_t max_size, std::byte *ptr,
                void (*write_handler)(memory_section s, uint64_t value),
                void (*read_handler)(memory_section s, uint64_t value));
};
