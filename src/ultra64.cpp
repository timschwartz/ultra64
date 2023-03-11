#include "ultra64.hpp"
#include <fstream>

void ultra64::load_config()
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

        this->save_config();
    }

    std::ifstream config_file(config_file_path);
    config_file >> this->config;
}

void ultra64::save_config()
{
    std::string home_dir(getenv("HOME"));
    std::string config_dir = home_dir + "/" + config_path;
    std::string config_file_path =  config_dir + "/config.json";

    std::ofstream config_file(config_file_path);
    config_file << std::setw(4) << this->config << std::endl;
    config_file.close();
}
