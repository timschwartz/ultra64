#pragma once

#include <wx/wx.h>
#include "n64/N64.hpp"
#include "RegistersWindow.hpp"
#include "DebuggerWindow.hpp"

const int ID_MEMORY_LISTBOX = 100;
const int ID_REGISTERS_LISTBOX = 101;
const int ID_CPU_STEP_COUNT = 102;
const int ID_DEBUGGER_PC = 103;

class MainWindow: public wxFrame
{
  public:
    MainWindow(const wxString &title, const wxPoint &pos, const wxSize &size);
  private:
    void OnExit(wxCommandEvent &event);
    void OnPaint(wxPaintEvent& event);

    void OnSelectPIFROM(wxCommandEvent &event);
    void OnDebugPIFROM(wxCommandEvent &event);
    void OnDebugROM(wxCommandEvent &event);
    void OnDebugRegisters(wxCommandEvent &event);

    wxDECLARE_EVENT_TABLE();
};


class wxUltra64 : public wxApp
{
  public:
    virtual bool OnInit();
    MainWindow *frame = nullptr;
    RegistersWindow *registers = nullptr;
    DebuggerWindow *debugger = nullptr;
    ultra64::N64 *n64 = nullptr;
};

enum
{
    ID_select_pif_rom = 0,
    ID_open_rom = 1,

    ID_debug_pif_rom = 10,
    ID_debug_rom = 11,
    ID_debug_registers = 12,

    BUTTON_cpu_step = 20,
    BUTTON_cpu_steps = 21
};

std::vector<std::string> render_debugger_registers(ultra64::N64 *n64);
