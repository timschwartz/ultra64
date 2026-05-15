#include "JoystickConfigDialog.hpp"

JoystickConfigDialog::JoystickConfigDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, const size_t controllerIndex)
    : wxDialog(parent, id, title, pos, size), controllerIndex(controllerIndex)
{
    joystickComboBox = new wxComboBox(this, wxID_ANY, wxEmptyString, wxPoint(20, 20), wxSize(500, 30));
    joystickInfo = new wxStaticText(this, wxID_ANY, wxT("Select a joystick"), wxPoint(20, 60), wxSize(300, 200));
    closeButton = new wxButton(this, wxID_ANY, wxT("Close"), wxPoint(20, 270), wxSize(100, 30));
    joystickCount = new wxStaticText(this, wxID_ANY, wxT("Joysticks: 0"), wxPoint(20, 90), wxSize(120, 30));

    ListJoysticks();

    // Dynamic event binding
    joystickComboBox->Bind(wxEVT_COMBOBOX, &JoystickConfigDialog::OnJoystickSelect, this);
    closeButton->Bind(wxEVT_BUTTON, &JoystickConfigDialog::OnClose, this);

    this->Bind(wxEVT_JOY_BUTTON_DOWN, &JoystickConfigDialog::OnJoystickEvent, this);
    this->Bind(wxEVT_JOY_BUTTON_UP, &JoystickConfigDialog::OnJoystickEvent, this);
    this->Bind(wxEVT_JOY_MOVE, &JoystickConfigDialog::OnJoystickEvent, this);
    this->Bind(wxEVT_JOY_ZMOVE, &JoystickConfigDialog::OnJoystickEvent, this);

    this->updateTimer = new wxTimer(this);
    this->updateTimer->Start(500); // Check every half second

    this->Bind(wxEVT_TIMER, &JoystickConfigDialog::OnTimer, this);
}

JoystickConfigDialog::~JoystickConfigDialog()
{
    // Dynamic event unbinding (optional, wxWidgets will clean up)
    joystickComboBox->Unbind(wxEVT_COMBOBOX, &JoystickConfigDialog::OnJoystickSelect, this);
    closeButton->Unbind(wxEVT_BUTTON, &JoystickConfigDialog::OnClose, this);
    
    this->Unbind(wxEVT_JOY_BUTTON_DOWN, &JoystickConfigDialog::OnJoystickEvent, this);
    this->Unbind(wxEVT_JOY_BUTTON_UP, &JoystickConfigDialog::OnJoystickEvent, this);
    this->Unbind(wxEVT_JOY_MOVE, &JoystickConfigDialog::OnJoystickEvent, this);
    this->Unbind(wxEVT_JOY_ZMOVE, &JoystickConfigDialog::OnJoystickEvent, this);

    // Stop and delete the timer
    this->updateTimer->Stop();
    delete this->updateTimer;

    // Unbind the timer event dynamically
    this->Unbind(wxEVT_TIMER, &JoystickConfigDialog::OnTimer, this);
}

void JoystickConfigDialog::ListJoysticks()
{
    if(this->numJoysticks == wxJoystick::GetNumberJoysticks())
    {
        return;
    }

    this->numJoysticks = wxJoystick::GetNumberJoysticks();
    std::string textJoystickCount = "Joysticks: " + std::to_string(this->numJoysticks);
    this->joystickCount->SetLabel(textJoystickCount);
    joystickComboBox->Clear();
    for (int i = 0; i < this->numJoysticks; i++)
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

void JoystickConfigDialog::OnClose(wxCommandEvent &event)
{
    this->Close();
}

void JoystickConfigDialog::OnJoystickEvent(wxJoystickEvent &event)
{
    this->ListJoysticks();
}

void JoystickConfigDialog::OnTimer(wxTimerEvent &event)
{
    std::cout << "JoystickConfigDialog::OnTimer" << std::endl;
    ListJoysticks();
}