#pragma once

#include <wx/wx.h>
#include <json/json.h>
#include "n64/N64.hpp"
#include "MainWindow.hpp"
#include "RegistersWindow.hpp"
#include "DebuggerWindow.hpp"

const int ID_MEMORY_LISTBOX = 100;
const int ID_REGISTERS_LISTBOX = 101;
const int ID_CPU_STEP_COUNT = 102;
const int ID_DEBUGGER_PC = 103;

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
    ID_select_pif_rom = 1,
    ID_open_rom = 2,

    ID_debug_pif_rom = 10,
    ID_debug_rom = 11,
    ID_debug_registers = 12,

    BUTTON_cpu_step = 20,
};

std::vector<std::string> render_debugger_registers(ultra64::N64 *n64);
Json::Value load_config();
void save_config(Json::Value config);
