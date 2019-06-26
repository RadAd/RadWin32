#include "MDIFrame.h"

#include "RegClass.h"
#include "WindowCreate.h"
#include "Menu.h"

//#include <tchar.h>

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

    LPCTSTR MDIFrame::GetWndClassName(HINSTANCE hInstance)
    {
        return MAKEINTATOM(GetMDIFrameAtom(hInstance));
    }

    Window* MDIFrame::CreateChild(Window* w, LPCTSTR WindowName)
    {
        w->CreateMDIChildWnd(WindowName, this);
        return w;
    }

    HMENU MDIFrame::FindWindowMenu()
    {
        MenuRef m = GetMenu();
#if 0
        for (int i = 0; i < m.GetItemCount(); ++i)
        {
            TCHAR label[1024];
            MenuRef hSubMenu = m.GetSubMenu(i);
            m.GetItemString(i, label, ARRAYSIZE(label), TRUE);
            if (hSubMenu.IsValid() && _tcscmp(label, _T("&Window")) == 0)
                return hSubMenu.GetHandle();
        }
        return NULL;
#else
        int i = m.GetItemCount() - 2;   // Assume window menu is second last (and help is last)
        return m.GetSubMenu(i).GetHandle();
#endif
    }

    LRESULT MDIFrame::OnCreate(LPCREATESTRUCT CreateStruct)
    {
        CLIENTCREATESTRUCT ccs = GetClientCreate();
        ::CreateWindow(_T("MDICLIENT"), (LPCTSTR) NULL,
            WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL,
            0, 0, 0, 0, GetHWND(), (HMENU) 0xCAC, CreateStruct->hInstance, (LPSTR) &ccs);

        return Window::OnCreate(CreateStruct);
    }
}
