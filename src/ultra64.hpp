#pragma once

#include <wx/wx.h>

class MainWindow: public wxFrame
{
  public:
    MainWindow(const wxString &title, const wxPoint &pos, const wxSize &size);
  private:
    void OnExit(wxCommandEvent &event);
    void OnPaint(wxPaintEvent& event);
    wxDECLARE_EVENT_TABLE();
};


class wxUltra64 : public wxApp
{
  public:
    virtual bool OnInit();
    MainWindow *frame = nullptr;
};
