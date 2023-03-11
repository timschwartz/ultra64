#pragma once

#include <wx/wx.h>

class MainWindow: public wxFrame
{
  public:
    MainWindow(const wxString &title, const wxPoint &pos, const wxSize &size);
  private:
    void OnExit(wxCommandEvent &event);
    void OnPaint(wxPaintEvent& event);

    void OnSelectPIFROM(wxCommandEvent &event);
    void OnOpenROM(wxCommandEvent &event);

    void OnSaveState(wxCommandEvent &event);
    void OnLoadState(wxCommandEvent &event);

    void OnDebugPIFROM(wxCommandEvent &event);
    void OnDebugROM(wxCommandEvent &event);
    void OnDebugRegisters(wxCommandEvent &event);

    wxDECLARE_EVENT_TABLE();
};
