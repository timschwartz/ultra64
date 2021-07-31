#include "MainWindow.hpp"
#include "ultra64.hpp"
#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <json/json.h>

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_MENU(wxID_EXIT,  MainWindow::OnExit)
    EVT_MENU(ID_select_pif_rom, MainWindow::OnSelectPIFROM)
    EVT_MENU(ID_open_rom, MainWindow::OnOpenROM)
    EVT_MENU(ID_debug_pif_rom, MainWindow::OnDebugPIFROM)
    EVT_MENU(ID_debug_rom, MainWindow::OnDebugROM)
    EVT_MENU(ID_debug_registers, MainWindow::OnDebugRegisters)
    EVT_PAINT(MainWindow::OnPaint)
wxEND_EVENT_TABLE()

DECLARE_APP(wxUltra64);

MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_select_pif_rom, "Select &PIF ROM", "");
    menuFile->Append(ID_open_rom, "&Open ROM", "");
    menuFile->Append(wxID_EXIT);

    wxMenu *menuDebug = new wxMenu;
    menuDebug->Append(ID_debug_pif_rom, "View &PIF ROM", "");
    menuDebug->Append(ID_debug_rom, "View &ROM", "");
    menuDebug->Append(ID_debug_registers, "View R&egisters", "");

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuDebug, "&Debug");
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

void MainWindow::OnSelectPIFROM(wxCommandEvent& event)
{
    std::string pif_path = wxGetApp().config["pif_rom_path"].asString();

    wxFileDialog rom_dialog(this, _("Open PIF ROM file"), pif_path, "",
                                "ROM files (*.bin)|*.bin|All files (*.*)|*.*",
                                wxFD_OPEN|wxFD_FILE_MUST_EXIST);

    if(wxID_CANCEL == rom_dialog.ShowModal()) return;

    std::string filename = rom_dialog.GetFilename().ToStdString();
    if(!filename.size()) return;
    wxGetApp().config["pif_rom"] = filename;
    wxGetApp().save_config();
}

void MainWindow::OnOpenROM(wxCommandEvent& event)
{
    std::string rom_path = wxGetApp().config["rom_path"].asString();

    wxFileDialog rom_dialog(this, _("Open N64 ROM file"), rom_path, "",
                                "N64 ROM files (*.n64;*.v64;*.z64;*.bin)|*.n64;*.v64;*.bin|All files (*.*)|*.*",
                                wxFD_OPEN|wxFD_FILE_MUST_EXIST);

    if(wxID_CANCEL == rom_dialog.ShowModal()) return;

    std::string directory = rom_dialog.GetDirectory().ToStdString();
    wxGetApp().config["rom_path"] = directory;
    wxGetApp().save_config();

    std::string rom = rom_dialog.GetPath().ToStdString();
    std::cout << "Opening " << rom << std::endl;

    start(rom);
}

void open_debugger()
{
    if(!wxGetApp().debugger) 
    {
        wxGetApp().debugger = new DebuggerWindow(wxGetApp().frame, 
                              "Ultra 64 Debugger", wxPoint(150, 50), wxSize(1000, 480));
    }

    wxGetApp().debugger->Layout();
}

void MainWindow::OnDebugPIFROM(wxCommandEvent& event)
{
    open_debugger();
    wxGetApp().debugger->view(0x1FC00000, 0x1FC007BF);
}

void MainWindow::OnDebugROM(wxCommandEvent& event)
{
    open_debugger();
    wxGetApp().debugger->view(0x10000000, 0x10008000);
}

void MainWindow::OnDebugRegisters(wxCommandEvent &event)
{
    if(!wxGetApp().registers)
    {
        wxGetApp().registers = new RegistersWindow(wxGetApp().frame,
                               "Registers", wxPoint(200, 50), wxSize(820, 450));
    }
    wxGetApp().registers->SetBackgroundColour(wxColour(*wxWHITE));
    wxGetApp().registers->Show(true);

    wxGetApp().registers->UpdateRegisters(render_debugger_registers(wxGetApp().n64));
}
