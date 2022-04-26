#pragma once
#ifndef WXCALC_SRC_WXCALCWINDOW_HPP
#define WXCALC_SRC_WXCALCWINDOW_HPP


#include <wx/wx.h>
#include <vector>
#include <string>


namespace WxCalc
{
    class WxCalcWindow :
        public wxFrame
    {
    public:
        WxCalcWindow();
    private:
        wxTextCtrl* m_display;
        std::vector<wxButton*> m_numButtons;
        wxButton* m_deciButton;
        wxButton* m_signButton;
        wxButton* m_addButton;
        wxButton* m_subButton;
        wxButton* m_mulButton;
        wxButton* m_divButton;
        wxButton* m_clsButton;
        wxButton* m_equalsButton;
        std::string m_preDecimal;
        std::string m_postDecimal;
        bool m_decimal;
        bool m_positive;
        double m_total;
        double m_last;
        int m_op;
        bool m_entryMode;
        double getCurrentValue();
        void updateDisplay();
        void clear();    
        void clearAndStore();
        void performOp();
        void showTotal();
        template<int NUM>
        void onNumButton(
            wxCommandEvent&);
        template<int OP>
        void onOpButton(
            wxCommandEvent&);
        wxDECLARE_EVENT_TABLE();
        WxCalcWindow(
            WxCalcWindow const& rhs);
        WxCalcWindow& operator=(
            WxCalcWindow const& rhs);
    };
}

#endif