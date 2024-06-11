#pragma once

#include <wx/wx.h>
#include <wx/joystick.h>

class JoystickConfigDialog : public wxDialog
{
public:
    JoystickConfigDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size);
    ~JoystickConfigDialog();

private:
    void OnJoystickSelect(wxCommandEvent& event);
    void OnClose(wxCommandEvent& event);
    
    wxComboBox* joystickComboBox;
    wxButton* closeButton;
    wxStaticText* joystickInfo;

    void ListJoysticks();
};
