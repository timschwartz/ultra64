#include "RegistersWindow.hpp"
#include "ultra64.hpp"

DECLARE_APP(wxUltra64)

RegistersWindow::RegistersWindow(wxWindow *parent, const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(parent, wxID_ANY, title, pos, size)
{
    wxFont reg_font(wxFontInfo(12).FaceName("Courier"));
    this->registers_listbox = new wxListBox(this, ID_REGISTERS_LISTBOX,
      wxPoint(20, 30), wxSize(780, 380));
    this->registers_listbox->SetFont(reg_font);
}

RegistersWindow::~RegistersWindow()
{
    wxGetApp().registers = NULL;
}

void RegistersWindow::UpdateRegisters(std::vector<std::string> data)
{
    this->registers_listbox->Clear();
    for(auto &line : data)
    {
        this->registers_listbox->Append(line.c_str());
    }
}
