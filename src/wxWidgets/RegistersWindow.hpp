#pragma once
#include <wx/wx.h>
#include <vector>

class RegistersWindow: public wxFrame
{
  public:
    RegistersWindow(wxWindow *parent, const wxString &title, const wxPoint &pos, const wxSize &size);
    ~RegistersWindow();
    void UpdateRegisters(std::vector<std::string> data);
    wxListBox *registers_listbox = NULL;
};
