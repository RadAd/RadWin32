#include "RegClass.h"

#include "Window.h"
#include "MDIFrame.h"
#include "..\WinError.h"

namespace rad
{
    RegClass::RegClass(HINSTANCE _hInstance, LPCTSTR ClassName)
    {
        ZeroMemory(this, sizeof(WNDCLASSEX));
        cbSize = sizeof(WNDCLASSEX);
        lpfnWndProc = Window::DefWndHandlerWindowProc;
        hInstance = _hInstance;
        hCursor = LoadCursor(NULL, IDC_ARROW);
        hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
        lpszClassName = ClassName;
        style = CS_HREDRAW | CS_VREDRAW;
    }

    ATOM RegClass::Register()
    {
        ATOM Atom = RegisterClassEx(this);
        if (Atom == 0)
            ThrowWinError(_T(__FUNCTION__));

        return Atom;
    }

    ATOM RegClass::GetSimple(HINSTANCE _hInstance)
    {
        static ATOM Atom = RegClass(_hInstance, _T("SimpleWindow")).Register();
        return Atom;
    }

    ATOM RegClass::GetMDIFrame(HINSTANCE _hInstance)
    {
        static ATOM Atom = GetMDIFrameClass(_hInstance).Register();
        return Atom;
    }

    ATOM RegClass::GetMDIChild(HINSTANCE _hInstance)
    {
        static ATOM Atom = GetMDIChildClass(_hInstance).Register();
        return Atom;
    }

    RegClass RegClass::GetMDIFrameClass(HINSTANCE _hInstance)
    {
        RegClass rc(_hInstance, _T("MDIFRAME"));
        rc.lpfnWndProc = MDIFrame::MDIFrameWndHandlerWindowProc;
        rc.hbrBackground = (HBRUSH) (COLOR_APPWORKSPACE + 1);
        rc.style = 0;
        return rc;
    }

    RegClass RegClass::GetMDIChildClass(HINSTANCE _hInstance)
    {
        RegClass rc(_hInstance, _T("MDICHILD"));
        rc.lpfnWndProc = MDIFrame::MDIChildWndHandlerWindowProc;
        rc.style = 0;
        return rc;
    }
}
