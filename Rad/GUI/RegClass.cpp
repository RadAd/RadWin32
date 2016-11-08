#include "RegClass.h"

namespace rad
{
    RegClass::RegClass(HINSTANCE _hInstance, LPCTSTR _ClassName)
    {
        ZeroMemory(this, sizeof(WNDCLASSEX));
        cbSize = sizeof(WNDCLASSEX);
        lpfnWndProc = WndHandlerWindowProc;
        hInstance = _hInstance;
        hCursor = LoadCursor(NULL, IDC_ARROW);
        hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
        lpszClassName = _ClassName;
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
}
