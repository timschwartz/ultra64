#include "ultra64.hpp"
#include "n64/N64.hpp"
#include "n64/PIFrom.hpp"
#include "n64/vr4300/vr4300.hpp"
#include "n64/rcp/RSP.hpp"
#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include "CRC.h"

const std::string config_path = ".config/ultra64";

wxIMPLEMENT_APP(wxUltra64);

void load_pif_rom(ultra64::N64 *n64);

bool wxUltra64::OnInit()
{
    this->frame = new MainWindow("Ultra64", wxPoint(20, 20), wxSize(320, 240));
    this->frame->Show(true);

    this->n64 = new ultra64::N64();
    try
    {
        load_pif_rom(this->n64);
    }
    catch (std::runtime_error &e)
    {
        wxGetApp().frame->SetStatusText(e.what(), 0);
    }

    if(wxGetApp().argc < 2) return true;

    std::string argv1 = wxGetApp().argv[1].ToStdString();
    start(argv1);

    return true;
}

const uint32_t cic_seed_nus_6102 = 0x00003F3F;
const uint32_t crc_nus_6102 = 0x90BB6CB5;

Json::Value load_config()
{
    std::string home_dir(getenv("HOME"));

    std::string config_dir = home_dir + "/" + config_path;
    if(!std::filesystem::is_directory(config_dir))
    {
        std::cout << config_dir << " doesn't exist, creating." << std::endl;
        std::filesystem::create_directory(config_dir);
    }

    std::string config_file_path =  config_dir + "/config.json";
    if(!std::filesystem::exists(config_file_path))
    {
        std::cout << config_file_path << " doesn't exist, creating." << std::endl;
        Json::Value config;

        config["pif_rom_path"] = config_dir;
        if(std::filesystem::exists(config_dir + "/pifdata.bin"))
        {
            config["pif_rom"] = "pifdata.bin";
        }
        else config["pif_rom"] = "";

        config["rom_path"] = home_dir;

        Json::StreamWriterBuilder builder;
        builder["indentation"] = " ";
        std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

        std::ofstream config_file(config_file_path);
        writer->write(config, &config_file);
        config_file.close();
    }

    Json::Value config;
    std::ifstream config_file(config_file_path);
    config_file >> config;
    return config;
}

void save_config(Json::Value config)
{
    std::string home_dir(getenv("HOME"));
    std::string config_dir = home_dir + "/" + config_path;
    std::string config_file_path =  config_dir + "/config.json";

    Json::StreamWriterBuilder builder;
    builder["indentation"] = " ";
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

    std::ofstream config_file(config_file_path);
    writer->write(config, &config_file);
    config_file.close();
}

void load_pif_rom(ultra64::N64 *n64)
{
    Json::Value config = load_config();

    std::string pif_rom_path = config["pif_rom_path"].asString();
    std::string pif_rom_file = config["pif_rom"].asString();
    if(!pif_rom_path.size() || !pif_rom_file.size())
    {
        throw std::runtime_error("Can't find PIF ROM. Load by clicking File -> Select PIF ROM.");
    }

    ultra64::PIFrom pifrom(pif_rom_path + "/" + pif_rom_file);

    uint32_t count = 0;
    uint32_t *p = (uint32_t *)pifrom.get_pointer();

    ultra64::map_memory(n64->mmu, "pif_rom", 0x1FC00000, 0x7C0, 0x7C0, new std::byte[0x7C0], nullptr, nullptr);

    while(count < 0x7C0)
    {
        n64->mmu->write_word(0x1FC00000 + count, p[count / 4]);
        count += 4;
    }

    std::cout << "Loaded " << pif_rom_path << "/" << pif_rom_file << std::endl;
}

std::vector<std::string> render_debugger_registers(ultra64::N64 *n64)
{
    std::vector<std::string> data;
    char temp[1024];

    uint32_t count = 0;
    while(count < 32)
    {
        sprintf(temp, "GPR[%.2d]=0x%.8lx  GPR[%.2d]=0x%.8lx  CP0[%.2d]=0x%.8lx  CP0[%.2d]=0x%.8lx",
                count, n64->cpu.GPR[count], count + 1, n64->cpu.GPR[count + 1],
                count, n64->cpu.CP0[count], count + 1, n64->cpu.CP0[count + 1]);
        data.push_back(temp);
        count += 2;
    }

    sprintf(temp, "HI=0x%.4x  LO=0x%.4x", n64->cpu.HI, n64->cpu.LO);
    data.push_back(temp);

    sprintf(temp, "PC = 0x%.8X", n64->cpu.get_PC());
    data.push_back(temp);

    return data;
}

void start(std::string filename)
{
    ultra64::N64 *n64 = wxGetApp().n64;

    n64->rom = new ultra64::ROM();
    n64->rom->Open(n64, filename);

    uint32_t crc = CRC::Calculate((n64->rom->get_pointer() + 0x40), 0xFC0, CRC::CRC_32());
    std::cout << "CRC = " << std::hex << crc << std::endl;

    switch(crc)
    {
        case crc_nus_6102:
            std::cout << "CIC seed: " << std::hex << cic_seed_nus_6102 << std::endl;
            try {
                wxGetApp().n64->mmu->write_word(0x1FC007C0 + 0x24, cic_seed_nus_6102);
            } catch(std::runtime_error &e) {
                std::cerr << e.what() << std::endl;
                return;
            }
            break;
        default:
            std::cerr << "No matching CIC seed for CRC: " << std::hex << crc << std::endl;
            break;
    }

    std::stringstream ss;

    ss << "Ultra 64 - " << n64->rom->header->name;

    wxGetApp().frame->SetTitle(ss.str());

    wxGetApp().n64->mmu->write_word_raw(SP_DMA_BUSY_REG, 0x0);

return;
    while(1)
    {
        try {
            wxGetApp().n64->cpu.step();
        } catch(std::runtime_error &e) {
            std::cerr << e.what() << std::endl;
            return;
        }

        if(wxGetApp().registers) wxGetApp().registers->UpdateRegisters(render_debugger_registers(wxGetApp().n64));
    }
}
