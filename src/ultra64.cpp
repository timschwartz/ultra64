#include "ultra64.hpp"
#include "n64/PIFrom.hpp"
#include <fstream>

ultra64::ultra64()
{
    this->cicSeeds[crc_nus_6102] = cic_seed_nus_6102;
}

std::string ultra64::config_load()
{
    std::string home_dir(getenv("HOME"));

    std::string config_dir = home_dir + "/" + config_path;
    if(!std::filesystem::is_directory(config_dir))
    {
        std::filesystem::create_directory(config_dir);
    }

    std::string config_file_path =  config_dir + "/config.json";
    if(!std::filesystem::exists(config_file_path))
    {
        this->config["pif_rom_path"] = config_dir;
        if(std::filesystem::exists(config_dir + "/pifdata.bin"))
        {
            this->config["pif_rom"] = "pifdata.bin";
        }
        else this->config["pif_rom"] = "";
        this->config["rom_path"] = home_dir;
        this->config["log_path"] = home_dir + "/ultra64.log";

        this->config_save();
    }

    std::ifstream config_file(config_file_path);
    config_file >> this->config;
    return config_file_path;
}

void ultra64::config_save()
{
    std::string home_dir(getenv("HOME"));
    std::string config_dir = home_dir + "/" + config_path;
    std::string config_file_path =  config_dir + "/config.json";

    std::ofstream config_file(config_file_path);
    config_file << std::setw(4) << this->config << std::endl;
    config_file.close();
}

std::string ultra64::config_get()
{
    return this->config.dump();
}

void ultra64::option_set(std::string opt, std::string value)
{
    this->config[opt] = value;
}

std::string ultra64::option_get(std::string opt)
{
    if(this->config[opt] == nullptr) return std::string("");
    return this->config[opt];
}

bool ultra64::is_valid_option(std::string opt)
{
    return std::any_of(
        this->validOptions.begin(),
        this->validOptions.end(),
        [opt](std::string const& s) { return s == opt; }
    );
}

bool ultra64::has_args(std::string opt)
{
    return std::any_of(
        this->hasArgs.begin(),
        this->hasArgs.end(),
        [opt](std::string const& s) { return s == opt; }
    );
}

void ultra64::n64_reset()
{
    this->rom.open(this->config["rom"]);

    this->map_memory("rom", 0x10000000, this->rom.size(), 0x0FBFFFFF, new std::byte[this->rom.size()], nullptr, nullptr);
    this->map_memory("pif_rom", 0x1FC00000, 0x7C0, 0x7C0, new std::byte[0x7C0], nullptr, nullptr);
    this->map_memory("pif_ram", 0x1FC007C0, 0x40, 0x40, new std::byte[0x40], &MMU::generic_write_logger, nullptr);
    this->map_memory("sp_dmem", 0x04000000, 0x1000, 0x1000, new std::byte[0x1000], &MMU::generic_write_logger, nullptr);
    this->map_memory("sp_imem", 0x04001000, 0x1000, 0x1000, new std::byte[0x1000], nullptr, nullptr);
//    this->map_memory("sp_registers", 0x04040000, 0x20, 0x20, new std::byte[0x20], &sp_register_write, &sp_register_read);
    this->map_memory("sp_registers", 0x04040000, 0x20, 0x20, new std::byte[0x20], &MMU::generic_write_logger, &MMU::generic_read_logger);
    this->map_memory("dp_registers", 0x04100000, 0x20, 0x20, new std::byte[0x20], &MMU::generic_write_logger, &MMU::generic_read_logger);
    this->map_memory("vi_registers", 0x04400000, 0x38, 0x38, new std::byte[0x38], nullptr, nullptr);
    this->map_memory("ai_registers", 0x04500000, 0x18, 0x18, new std::byte[0x18], nullptr, nullptr);
    this->map_memory("pi_registers", 0x04600000, 0x34, 0x34, new std::byte[0x34], &MMU::generic_write_logger, nullptr);
//    this->map_memory("si_registers", 0x04800000, 0x1C, 0x1C, new std::byte[0x1C], nullptr, &si_register_read);
    this->map_memory("si_registers", 0x04800000, 0x1C, 0x1C, new std::byte[0x1C], nullptr, &MMU::generic_read_logger);
    this->map_memory("dd_ipl_rom", 0x06000000, 0x400000, 0x400000, new std::byte[0x400000], nullptr, nullptr);

    PIFrom pifrom(this->config["pif-rom"]);
    uint32_t *p = (uint32_t *)pifrom.get_pointer();
    for(size_t count = 0; count < 0x7C0; count +=4)
    {
        this->n64.mmu.write_word_raw(0x1FC00000 + count, *p++);
    }

    uint32_t *r = (uint32_t *)this->rom.get_pointer();
    for(size_t count = 0; count < this->rom.size(); count += 4)
    {
        this->n64.mmu.write_word_raw(0x10000000 + count, *r++);
    }

    this->n64.mmu.write_word(0x1FC007C0 + 0x24, this->cicSeeds[this->rom.crc()]);
    this->n64.mmu.write_word_raw(SP_STATUS_REG, SP_STATUS_HALT);
    this->n64.mmu.write_word_raw(SP_DMA_BUSY_REG, 0x0);

    try {
        for(;;) this->n64.cpu.step();
    } catch(std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return;
    }
}

void ultra64::map_memory(std::string name, uint32_t addr, uint32_t size, uint32_t max_size, std::byte *ptr,
                void (*write_handler)(memory_section s, uint64_t value),
                void (*read_handler)(memory_section s, uint64_t value))
{
    memory_section s;
    
    s.offset = addr;
    s.size = size;
    s.max_size = max_size;
    s.ptr = ptr;
    s.write_handler = write_handler;
    s.read_handler = read_handler;
    memset(s.ptr, 0, s.size);

    this->n64.mmu.memory_register(name, s);
}

nlohmann::json ultra64::state_save()
{
    nlohmann::json state;
    state["cpu"] = this->n64.cpu.save_state();
    return state;
}

void ultra64::state_load(nlohmann::json state)
{
    this->n64.cpu.load_state(state["cpu"]);
}

void ultra64::step()
{
    this->n64.cpu.step();
}

uint32_t ultra64::read_word_raw(uint32_t addr)
{
    return this->n64.mmu.read_word_raw(addr);
}
