#include "JoystickConfigDialog.hpp"

JoystickConfigDialog::JoystickConfigDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxDialog(parent, id, title, pos, size)
{
    joystickComboBox = new wxComboBox(this, wxID_ANY, wxEmptyString, wxPoint(20, 20), wxSize(200, 30));
    joystickInfo = new wxStaticText(this, wxID_ANY, wxT("Select a joystick"), wxPoint(20, 60), wxSize(300, 200));
    closeButton = new wxButton(this, wxID_ANY, wxT("Close"), wxPoint(20, 270), wxSize(100, 30));

    ListJoysticks();

    // Dynamic event binding
    joystickComboBox->Bind(wxEVT_COMBOBOX, &JoystickConfigDialog::OnJoystickSelect, this);
    closeButton->Bind(wxEVT_BUTTON, &JoystickConfigDialog::OnClose, this);
}

JoystickConfigDialog::~JoystickConfigDialog()
{
    // Dynamic event unbinding (optional, wxWidgets will clean up)
    joystickComboBox->Unbind(wxEVT_COMBOBOX, &JoystickConfigDialog::OnJoystickSelect, this);
    closeButton->Unbind(wxEVT_BUTTON, &JoystickConfigDialog::OnClose, this);
}

void JoystickConfigDialog::ListJoysticks()
{
    joystickComboBox->Clear();
    int numJoysticks = wxJoystick::GetNumberJoysticks();
    for (int i = 0; i < numJoysticks; i++)
    {
        wxJoystick joystick(i);
        joystickComboBox->Append(wxString::Format("Joystick %d: %s", i, joystick.GetProductName()));
    }
}

void JoystickConfigDialog::OnJoystickSelect(wxCommandEvent& event)
{
    int selection = joystickComboBox->GetSelection();
    if (selection != wxNOT_FOUND)
    {
        wxJoystick joystick(selection);
        wxString info;
        info.Printf(wxT("Name: %s\nAxes: %d\nButtons: %d\nIsOk: %d"),
            joystick.GetProductName(),
            joystick.GetMaxAxes(),
            joystick.GetMaxButtons(),
            joystick.IsOk());
        joystickInfo->SetLabel(info);
    }
}

void JoystickConfigDialog::OnClose(wxCommandEvent& event)
{
    Close();
}
