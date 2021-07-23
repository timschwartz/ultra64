#include "ultra64.hpp"
#include "MMU.hpp"
#include "ROM.hpp"
#include "PIFrom.hpp"
#include "vr4300/vr4300.hpp"
#include "rcp/rsp.hpp"
#include <iostream>
#include "CRC.h"

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_MENU(wxID_EXIT,  MainWindow::OnExit)
    EVT_PAINT(MainWindow::OnPaint)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(wxUltra64);

void start(std::string filename);

MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(wxID_EXIT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    SetMenuBar(menuBar);

    CreateStatusBar(1);
}

void MainWindow::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
}

void MainWindow::OnExit(wxCommandEvent& event)
{
    Close(true);
}

bool wxUltra64::OnInit()
{
    this->frame = new MainWindow("Ultra64", wxPoint(20, 20), wxSize(640, 480));
    this->frame->Show(true);

    if(wxGetApp().argc < 2) return true;

    std::string argv1 = wxGetApp().argv[1].ToStdString();
    start(argv1);

    return true;
}

const uint32_t cic_seed_nus_6102 = 0x00003F3F;
const uint32_t crc_nus_6102 = 0x90BB6CB5;

void start(std::string filename)
{
    ultra64::PIFrom pifrom("pifdata.bin");

    uint32_t count = 0;
    uint32_t *p = (uint32_t *)pifrom.get_pointer();

    ultra64::MMU mmu;
    ultra64::memory_section s;
    s.offset = 0x1FC00000;
    s.size = 0x7C0;
    s.max_size = s.size;
    s.ptr = new std::byte[s.size]; // pifrom.get_pointer();
    mmu.register_memory("pif_rom", s);

    while(count < 0x7C0)
    {
        mmu.write_word(0x1FC00000 + count, p[count / 4]);
        count += 4;
    }

    ultra64::ROM rom(filename);
    s.offset = 0x10000000;
    s.max_size = 0x0FBFFFFF;
    s.size = rom.size();
    s.ptr = rom.get_pointer();
    mmu.register_memory("rom", s);

    uint32_t crc = CRC::Calculate(rom.get_pointer() + 0x40, 0xFC0, CRC::CRC_32());
    std::cout << "CRC = " << std::hex << crc << std::endl;

    switch(crc)
    {
        case crc_nus_6102:
            std::cout << "CIC seed: " << std::hex << cic_seed_nus_6102 << std::endl;
            try {
                mmu.write_word(0x1FC007C0 + 0x24, cic_seed_nus_6102);
            } catch(std::runtime_error &e) {
                std::cerr << e.what() << std::endl;
                return;
            }
            break;
        default:
            std::cerr << "No matching CIC seed for CRC: " << crc << std::endl;
            break;
    }

    ultra64::vr4300 cpu(mmu);
    ultra64::rsp rsp(mmu);


    mmu.write_word(SP_DMA_BUSY_REG, 0x0);

    while(1)
    {
        try {
            cpu.step();
        } catch(std::runtime_error &e) {
            std::cerr << e.what() << std::endl;
            return;
        }
    }
}
