#pragma once
#include <wx/wx.h>

class DebuggerWindow: public wxFrame
{
  public:
    DebuggerWindow(wxWindow *parent, const wxString &title, const wxPoint &pos, const wxSize &size);
    ~DebuggerWindow();
    wxListBox *memory_listbox = NULL;
    void view(uint32_t start_addr, uint32_t end_addr);
    wxTextCtrl *debugger_pc = NULL;
    wxButton *cpu_step = NULL;
    void OnCPUStep(wxCommandEvent &event);
    wxTextCtrl *cpu_steps_count = NULL;
    wxButton *cpu_steps = NULL;
    void OnCPUSteps(wxCommandEvent &event);
    wxDECLARE_EVENT_TABLE();
};
