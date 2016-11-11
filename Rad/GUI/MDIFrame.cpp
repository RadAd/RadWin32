#include "MDIFrame.h"

#include "RegClass.h"
#include "WindowCreate.h"

namespace rad
{
    RegClass MDIFrame::GetMDIFrameReg(HINSTANCE _hInstance)
    {
        RegClass rc(_hInstance, _T("MDIFRAME"), MDIFrameWndHandlerWindowProc);
        rc.hbrBackground = (HBRUSH) (COLOR_APPWORKSPACE + 1);
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

    void MDIFrame::CreateWnd(HINSTANCE hInstance, LPCTSTR WindowName, HWND hParent)
    {
        CreateWnd(WindowCreate(hInstance, GetMDIFrameAtom(hInstance)), WindowName, hParent);
    }

    Window* MDIFrame::CreateChild(Window* w, LPCTSTR WindowName)
    {
        HINSTANCE hInstance = (HINSTANCE) GetWindowLongPtr(GWLP_HINSTANCE);

        MDIChildCreate c(hInstance, GetMDIChildAtom(hInstance));
        c.Create(GetMDIClient().GetHWND(), WindowName, w);
        return w;
    }
}
