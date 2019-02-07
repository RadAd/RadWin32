#include "RegClass.h"

#include "..\WinError.h"

namespace rad
{
    RegClass::RegClass(HINSTANCE _hInstance, LPCTSTR ClassName, WNDPROC WndProc)
    {
        ZeroMemory(this, sizeof(WNDCLASSEX));
        cbSize = sizeof(WNDCLASSEX);
        lpfnWndProc = WndProc;
        hInstance = _hInstance;
        hCursor = LoadCursor(NULL, IDC_ARROW);
        SetBackgroundColorType(COLOR_WINDOW);
        lpszClassName = ClassName;
        style = CS_HREDRAW | CS_VREDRAW;
    }

    ATOM RegClass::Register() const
    {
        ATOM Atom = RegisterClassEx(this);
        if (Atom == 0)
            ThrowWinError(_T(__FUNCTION__));

        return Atom;
    }
}
