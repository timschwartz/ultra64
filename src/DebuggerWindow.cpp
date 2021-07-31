#include "DebuggerWindow.hpp"
#include "ultra64.hpp"
#include "n64/instruction.hpp"

wxBEGIN_EVENT_TABLE(DebuggerWindow, wxFrame)
    EVT_BUTTON(BUTTON_cpu_step, DebuggerWindow::OnCPUStep)
    EVT_SIZE(DebuggerWindow::OnResize)
wxEND_EVENT_TABLE()

DECLARE_APP(wxUltra64)

DebuggerWindow::DebuggerWindow(wxWindow *parent, const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(parent, wxID_ANY, title, pos, size)
{
    this->SetBackgroundColour(wxColour(*wxWHITE));
    this->Show(true);

    wxFont font(wxFontInfo(14).Bold());
    wxStaticText *label_memory_address = new wxStaticText(this, wxID_ANY, wxT("Address      Data"), wxPoint(20, 5));
    label_memory_address->SetFont(font);

    wxFont mem_font(wxFontInfo(12).FaceName("Courier"));
    this->memory_listbox = new wxListBox(this, ID_MEMORY_LISTBOX, wxPoint(20, 30), wxSize(750, 380));
    this->memory_listbox->SetFont(mem_font);

    this->label_pc = new wxStaticText(this, wxID_ANY, wxT("PC"), wxPoint(850, 30), wxSize(100, 50));
    this->label_pc->SetFont(font);
    this->debugger_pc = new wxTextCtrl(this, ID_DEBUGGER_PC, "0x0000 0000", wxPoint(850, 60), wxSize(120, 25), 0);

    this->label_steps = new wxStaticText(this, wxID_ANY, wxT("Step count"), wxPoint(850, 100), wxSize(150, 50));
    this->label_steps->SetFont(font);
    this->cpu_steps_count = new wxTextCtrl(this, ID_CPU_STEP_COUNT, "1", wxPoint(850, 130), wxSize(120, 25), 0);
    this->cpu_step = new wxButton(this, BUTTON_cpu_step, _T("Step"), wxPoint(850, 170), wxDefaultSize, 0);

    this->SendSizeEvent();
}

DebuggerWindow::~DebuggerWindow()
{
    wxGetApp().debugger = NULL;
}

void DebuggerWindow::OnResize(wxSizeEvent &event)
{
    if(!this->label_pc) return;

    const int margin = 30;
    const int control_width = 120;

    int width;
    width = event.GetSize().GetWidth();

    int x, y;

    this->label_pc->GetPosition(&x, &y);
    this->label_pc->Move(width - control_width - margin, y);

    this->debugger_pc->GetPosition(&x, &y);
    this->debugger_pc->Move(width - control_width - margin, y);

    this->label_steps->GetPosition(&x, &y);
    this->label_steps->Move(width - control_width - margin, y);

    this->cpu_steps_count->GetPosition(&x, &y);
    this->cpu_steps_count->Move(width - control_width - margin, y);

    this->cpu_step->GetPosition(&x, &y);
    this->cpu_step->Move(width - control_width - margin, y);
}

void DebuggerWindow::OnCPUStep(wxCommandEvent& event)
{
    ultra64::N64 *n64 = wxGetApp().n64;

    wxString contents = this->cpu_steps_count->GetValue();
    uint32_t steps = wxAtoi(contents), count = 0;

    char message[1024];
    sprintf(message, "%d", steps);
    cpu_steps_count->Clear();
    cpu_steps_count->AppendText(message);

    while(count < steps)
    {
        try
        {
            n64->cpu.step();
        }
        catch(std::string e)
        {
            wxGetApp().frame->SetStatusText(e, 0);
        }
        count++;
    }

    sprintf(message, "0x%.4X %.4X", n64->cpu.get_PC() >> 16, n64->cpu.get_PC() & 0xFFFF);
    debugger_pc->Clear();
    debugger_pc->AppendText(message);

    if(wxGetApp().registers != NULL) wxGetApp().registers->UpdateRegisters(render_debugger_registers(wxGetApp().n64));
}

void DebuggerWindow::view(uint32_t start_addr, uint32_t end_addr)
{
    ultra64::N64 *n64 = wxGetApp().n64;

    char message[1024];
    std::string msg;
    uint32_t value, addr = start_addr;

    sprintf(message, "0x%.4X %.4X", n64->cpu.get_PC() >> 16, n64->cpu.get_PC() & 0xFFFF);
    this->debugger_pc->Clear();
    this->debugger_pc->AppendText(message);

    auto const max_length = 60;
    while(addr < end_addr)
    {
        try
        {
            value = n64->mmu->read_word_raw(addr);
            sprintf(message, "%.4X %.4X   %.2X %.2X %.2X %.2X   ", (addr >> 16), (addr & 0xFFFF), 
                   value >> 24, (value >> 16) & 0xFF, (value >> 8) & 0xFF, value & 0xFF);
            ultra64::instruction i(value);
            msg = message + i.to_string();

            // Pad with whitespace
            while(msg.size() <= max_length) msg += " ";

            // Show raw bytes in memory
            for(int i = 3; i >= 0; i--)
            {
              char temp = (value >> (i * 8)) & 0xFF;

              // Don't try to print not-printable characters
              if(temp < 32) temp = '.';

              sprintf(message, "%c ", temp);
              msg += message;
            }
            memory_listbox->Append(msg.c_str());
        }
        catch(std::string e)
        {
            memory_listbox->Append(e.c_str());
            return;
        }
        addr += 4;
    }
}
