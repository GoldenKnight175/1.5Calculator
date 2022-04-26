#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/display.h>
#include "WxCalcWindow.h"


namespace WxCalc
{
    class App :
        public wxApp
    {
    public:
        virtual bool OnInit()
        {
            WxCalcWindow* w = new WxCalcWindow();
            w->Show(true);
            return true;
        }
    };
}
wxIMPLEMENT_APP(WxCalc::App);