#pragma once

#include <wx/wx.h>
#include <wx/joystick.h>

class JoystickConfigDialog : public wxDialog
{
public:
    JoystickConfigDialog(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, const size_t controllerIndex = 0);
    ~JoystickConfigDialog();

private:
    void OnJoystickSelect(wxCommandEvent &event);
    void OnClose(wxCommandEvent &event);
    void OnJoystickEvent(wxJoystickEvent &event);
    void OnTimer(wxTimerEvent &event);
    
    wxComboBox* joystickComboBox;
    wxButton* closeButton;
    wxStaticText *joystickInfo;
    wxStaticText *joystickCount;
    wxTimer* updateTimer;

    void ListJoysticks();
    size_t controllerIndex;
    int numJoysticks = 0;
};
