#pragma once
#include <wx/wx.h>

class DebuggerWindow: public wxFrame
{
  public:
    DebuggerWindow(wxWindow *parent, const wxString &title, const wxPoint &pos, const wxSize &size);
    ~DebuggerWindow();
    wxListBox *memory_listbox = nullptr;
    void view(uint32_t start_addr, uint32_t end_addr);
    wxStaticText *label_pc = nullptr;
    wxTextCtrl *debugger_pc = nullptr;
    wxStaticText *label_steps = nullptr;
    wxButton *cpu_step = nullptr;
    void OnCPUStep(wxCommandEvent &event);
    void OnResize(wxSizeEvent &event);
    wxTextCtrl *cpu_steps_count = nullptr;
    wxButton *cpu_steps = nullptr;
    wxDECLARE_EVENT_TABLE();
};
