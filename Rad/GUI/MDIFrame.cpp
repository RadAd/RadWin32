#include "MDIFrame.h"

#include "RegClass.h"
#include "WindowCreate.h"

namespace rad
{
    RegClass MDIFrame::GetMDIFrameReg(HINSTANCE _hInstance)
    {
        RegClass rc(_hInstance, _T("MDIFRAME"), MDIFrameWndHandlerWindowProc);
        rc.SetBackgroundColorType(COLOR_APPWORKSPACE);
        rc.style = 0;
        return rc;
    }

    ATOM MDIFrame::GetMDIFrameAtom(HINSTANCE hInstance)
    {
        static ATOM Atom = GetMDIFrameReg(hInstance).Register();
        return Atom;
    }

    RegClass MDIFrame::GetMDIChildReg(HINSTANCE _hInstance)
    {
        RegClass rc(_hInstance, _T("MDICHILD"), MDIChildWndHandlerWindowProc);
        rc.style = 0;
        return rc;
    }

    ATOM MDIFrame::GetMDIChildAtom(HINSTANCE hInstance)
    {
        static ATOM Atom = GetMDIChildReg(hInstance).Register();
        return Atom;
    }

    LPCTSTR MDIFrame::GetWndClassName(HINSTANCE hInstance)
    {
        return MAKEINTATOM(GetMDIFrameAtom(hInstance));
    }

    Window* MDIFrame::CreateChild(Window* w, LPCTSTR WindowName)
    {
#if 0
        HINSTANCE hInstance = (HINSTANCE) GetWindowLongPtr(GWLP_HINSTANCE);

        MDIChildCreate c(hInstance);
        c.Create(GetMDIClient().GetHWND(), WindowName, w, w->GetMDIChildClassName(hInstance));
#else
        w->CreateMDIChildWnd(WindowName, this);
#endif
        return w;
    }
}
