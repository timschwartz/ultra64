#include "wxUltra64.hpp"
#include "../n64/N64.hpp"
#include "../n64/PIFrom.hpp"
#include "../n64/vr4300/vr4300.hpp"
#include "../n64/rcp/RSP.hpp"
#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include "../n64/CRC.h"

const std::string config_path = ".config/ultra64";

wxIMPLEMENT_APP(wxUltra64);

bool wxUltra64::OnInit()
{
    this->frame = new MainWindow("Ultra64", wxPoint(20, 20), wxSize(320, 240));
    this->frame->Show(true);

    std::string argv1;
    if(wxGetApp().argc > 1)
    {
        argv1 = wxGetApp().argv[1].ToStdString();
    }

    std::cout << "Loaded config from " << wxGetApp().config_load() << std::endl;
    std::cout << wxGetApp().config_get() << std::endl;

    start(argv1);

    return true;
}

std::vector<std::string> render_registers(N64 *n64)
{
    std::vector<std::string> data;
    char temp[1024];

    uint32_t count = 0;
    while(count < 32)
    {
        sprintf(temp, "GPR[%.2d]=0x%.16lX  GPR[%.2d]=0x%.16lX  CP0[%.2d]=0x%.16lX  CP0[%.2d]=0x%.16lX",
                count, n64->cpu.GPR[count], count + 1, n64->cpu.GPR[count + 1],
                count, n64->cpu.CP0[count], count + 1, n64->cpu.CP0[count + 1]);
        data.push_back(temp);
        count += 2;
    }

    sprintf(temp, "HI=0x%.4x  LO=0x%.4x", n64->cpu.HI, n64->cpu.LO);
    data.push_back(temp);

    sprintf(temp, "PC = 0x%.8X", n64->cpu.PC);
    data.push_back(temp);

    return data;
}

void start(std::string filename)
{
    if(filename.size() == 0) return;

    wxGetApp().option_set("rom", filename);

    std::cout << wxGetApp().config_get() << std::endl;

    wxGetApp().n64_reset();

#if 0
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

        if(wxGetApp().registers) wxGetApp().registers->UpdateRegisters(render_registers(wxGetApp().n64));
    }
#endif
}
