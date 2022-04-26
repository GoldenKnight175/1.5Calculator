#include "WxCalcWindow.h"
#include "Calc.h"
#include <stdexcept>
#include <cstdint>


namespace WxCalc
{
    namespace
    {
        const int op_offset = 1024;
        enum event_ids {
            ID_0_BUTTON = 0,
            ID_1_BUTTON = 1,
            ID_2_BUTTON = 2,
            ID_3_BUTTON = 3,
            ID_4_BUTTON = 4,
            ID_5_BUTTON = 5,
            ID_6_BUTTON = 6,
            ID_7_BUTTON = 7,
            ID_8_BUTTON = 8,
            ID_9_BUTTON = 9,
            ID_DECI_BUTTON,
            ID_SIGN_BUTTON,
            ID_ADD_BUTTON = op_offset + Calculator::Add,
            ID_SUB_BUTTON = op_offset + Calculator::Sub,
            ID_MUL_BUTTON = op_offset + Calculator::Mult,
            ID_DIV_BUTTON = op_offset + Calculator::Div,
            ID_CLS_BUTTON,
            ID_EQUAL_BUTTON
        };
    }
    double WxCalcWindow::getCurrentValue()
    {
        try {
            return std::stod(std::string(m_display->GetValue().mb_str()));
        }
        catch (const std::invalid_argument& e) {
            return 0.0;
        }
    }
    void WxCalcWindow::updateDisplay()
    {
        std::string numStr("");
        if (!m_positive) {
            numStr += "-";
        }
        numStr += m_preDecimal;
        if (m_decimal) {
            numStr += "." + m_postDecimal;
        }
        m_display->SetValue(numStr);
    }
    void WxCalcWindow::clear()
    {
        m_preDecimal.assign("");
        m_postDecimal.assign("");
        m_decimal = false;
        m_positive = true;
    }
    void WxCalcWindow::clearAndStore()
    {
        m_total = getCurrentValue();
        clear();
        updateDisplay();
    }
    void WxCalcWindow::performOp()
    {
        if (m_op < 0){}
        else {
            m_total = Calculator::performOperation(m_total, m_last, m_op);
            showTotal();
        }
    }
    void WxCalcWindow::showTotal()
    {
        clear();
        if (m_total == static_cast<int64_t>(m_total)) {
            m_display->SetValue(std::to_string(static_cast<int64_t>(m_total)));
        }
        else {
            m_display->SetValue(std::to_string(m_total));
        }
    }
    template<int NUM>
    void WxCalcWindow::onNumButton(
        wxCommandEvent&)
    {
        m_entryMode = true;
        if (!m_decimal) {
            m_preDecimal += std::to_string(NUM);
        }
        else {
            m_postDecimal += std::to_string(NUM);
        }
        updateDisplay();
    }
    template<int OP>
    void WxCalcWindow::onOpButton(
        wxCommandEvent&)
    {
        switch (OP) {
        case ID_DECI_BUTTON: {
            m_decimal = true;
            m_entryMode = true;
            updateDisplay();
            break;
        }
        case ID_SIGN_BUTTON: {
            m_positive = !m_positive;
            m_entryMode = true;
            updateDisplay();
            break;
        }
        case ID_ADD_BUTTON:
        case ID_SUB_BUTTON:
        case ID_MUL_BUTTON:
        case ID_DIV_BUTTON: {
            if (m_entryMode) {
                m_last = getCurrentValue();
                m_entryMode = false;
            }
            performOp();
            clearAndStore();
            showTotal();
            m_op = OP - op_offset;
            break;
        }
        case ID_CLS_BUTTON: {
            clear();
            m_entryMode = true;
            m_total = 0;
            m_last = 0;
            m_op = -1;
            updateDisplay();
            break;
        }
        case ID_EQUAL_BUTTON: {
            if (m_entryMode) {
                m_last = getCurrentValue();
                m_entryMode = false;
            }
            performOp();
            break;
        }
        }
    }
#define NUM_EVENT(id) \
  EVT_BUTTON(id, WxCalcWindow::onNumButton<id>)
#define OP_EVENT(id) \
  EVT_BUTTON(id, WxCalcWindow::onOpButton<id>)
    wxBEGIN_EVENT_TABLE(WxCalcWindow, wxFrame)
        NUM_EVENT(ID_0_BUTTON)
        NUM_EVENT(ID_1_BUTTON)
        NUM_EVENT(ID_2_BUTTON)
        NUM_EVENT(ID_3_BUTTON)
        NUM_EVENT(ID_4_BUTTON)
        NUM_EVENT(ID_5_BUTTON)
        NUM_EVENT(ID_6_BUTTON)
        NUM_EVENT(ID_7_BUTTON)
        NUM_EVENT(ID_8_BUTTON)
        NUM_EVENT(ID_9_BUTTON)
        OP_EVENT(ID_DECI_BUTTON)
        OP_EVENT(ID_SIGN_BUTTON)
        OP_EVENT(ID_ADD_BUTTON)
        OP_EVENT(ID_SUB_BUTTON)
        OP_EVENT(ID_MUL_BUTTON)
        OP_EVENT(ID_DIV_BUTTON)
        OP_EVENT(ID_CLS_BUTTON)
        OP_EVENT(ID_EQUAL_BUTTON)
        wxEND_EVENT_TABLE()
        WxCalcWindow::WxCalcWindow() :
        wxFrame(NULL, wxID_ANY, "Wx-Calc", wxPoint(100, 100), wxDefaultSize, \
            wxCLOSE_BOX | wxCAPTION),
        m_display(nullptr),
        m_numButtons(),
        m_deciButton(nullptr),
        m_signButton(nullptr),
        m_addButton(nullptr),
        m_subButton(nullptr),
        m_mulButton(nullptr),
        m_divButton(nullptr),
        m_clsButton(nullptr),
        m_equalsButton(nullptr),
        m_preDecimal(),
        m_postDecimal(),
        m_decimal(false),
        m_positive(true),
        m_total(0),
        m_last(0),
        m_op(-1),
        m_entryMode(true)
    {
        wxBoxSizer* SizeofTop = new wxBoxSizer(wxVERTICAL);
        m_display = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, \
            wxDefaultSize, wxTE_RIGHT);
        m_display->Disable();
        wxBoxSizer* DisplayS = new wxBoxSizer(wxHORIZONTAL);
        DisplayS->Add(m_display, 1, wxEXPAND | wxALL, 0);
        SizeofTop->Add(DisplayS, 0, wxEXPAND, 0);
        SizeofTop->AddSpacer(10);
        wxGridSizer* ButtonSize = new wxGridSizer(4, 0, 0);
        wxSize SizeofButton(64, 64);
        m_numButtons.resize(10);
        for (int num = 0; num < 10; ++num) {
            m_numButtons[num] = new wxButton(this, num, std::to_string(num), \
                wxDefaultPosition, SizeofButton);
        }
        m_addButton = new wxButton(this, ID_ADD_BUTTON, "+", wxDefaultPosition, \
            SizeofButton);
        m_subButton = new wxButton(this, ID_SUB_BUTTON, "-", wxDefaultPosition, \
            SizeofButton);
        m_mulButton = new wxButton(this, ID_MUL_BUTTON, "*", wxDefaultPosition, \
            SizeofButton);
        m_divButton = new wxButton(this, ID_DIV_BUTTON, "/", wxDefaultPosition, \
            SizeofButton);
        m_clsButton = new wxButton(this, ID_CLS_BUTTON, "CLS", wxDefaultPosition, \
            SizeofButton);
        m_equalsButton = new wxButton(this, ID_EQUAL_BUTTON, "=", \
            wxDefaultPosition, SizeofButton);
        m_deciButton = new wxButton(this, ID_DECI_BUTTON, ".", wxDefaultPosition, \
            SizeofButton);
        m_signButton = new wxButton(this, ID_SIGN_BUTTON, "+/-", wxDefaultPosition, \
            SizeofButton);
        for (int num = 1; num <= 3; ++num) {
            ButtonSize->Add(m_numButtons[num], 1, wxEXPAND, 0);
        }
        ButtonSize->Add(m_addButton, 1, wxEXPAND, 0);
        for (int num = 4; num <= 6; ++num) {
            ButtonSize->Add(m_numButtons[num], 1, wxEXPAND, 0);
        }
        ButtonSize->Add(m_subButton, 1, wxEXPAND, 0);
        for (int num = 7; num <= 9; ++num) {
            ButtonSize->Add(m_numButtons[num], 1, wxEXPAND, 0);
        }
        ButtonSize->Add(m_mulButton, 1, wxEXPAND, 0);
        ButtonSize->Add(m_numButtons[0], 1, wxEXPAND, 0);
        ButtonSize->Add(m_deciButton, 1, wxEXPAND, 0);
        ButtonSize->Add(m_signButton, 1, wxEXPAND, 0);
        ButtonSize->Add(m_divButton, 1, wxEXPAND, 0);
        ButtonSize->AddStretchSpacer();
        ButtonSize->AddStretchSpacer();
        ButtonSize->Add(m_clsButton, 1, wxEXPAND, 0);
        ButtonSize->Add(m_equalsButton, 1, wxEXPAND, 0);
        SizeofTop->Add(ButtonSize, 1, wxEXPAND, 0);
        SetSizerAndFit(SizeofTop);
    }
}
