#ifndef __WindowCREATE_H__
#define __WindowCREATE_H__

#include "RegClass.h"
#include "..\WinError.h"

namespace rad
{
    class WindowCreate
    {
    public:
        WindowCreate(HINSTANCE _hInstance, LPCTSTR _WindowName, HWND _hParent = NULL)
        {
            hInstance = _hInstance;
            ClassName = MAKEINTATOM(RegClass::GetSimple(_hInstance));
            WindowName = _WindowName;
            hParent = _hParent;
        }

        WindowCreate(HINSTANCE _hInstance, LPCTSTR _WindowName, LPCTSTR _ClassName, HWND _hParent = NULL)
        {
            hInstance = _hInstance;
            ClassName = _ClassName;
            WindowName = _WindowName;
            hParent = _hParent;
        }

        WindowCreate(HINSTANCE _hInstance, LPCTSTR _WindowName, ATOM Class, HWND _hParent = NULL)
        {
            hInstance = _hInstance;
            ClassName = MAKEINTATOM(Class);
            WindowName = _WindowName;
            hParent = _hParent;
        }

        ~WindowCreate() { }

        HWND Create(LPVOID data) const
        {
            HWND hWnd = CreateWindowEx(ExStyle,
                ClassName,
                WindowName,
                Style,
                x, y,
                Width, Height,
                hParent, hMenu,
                hInstance,
                data);

            if (hWnd == NULL)
                ThrowWinError(_T(__FUNCTION__));

            return hWnd;
        }

        HINSTANCE       hInstance = NULL;
        LPCTSTR         ClassName = nullptr;
        LPCTSTR         WindowName = nullptr;
        DWORD           Style = WS_OVERLAPPEDWINDOW;
        DWORD           ExStyle = 0;
        int             x = CW_USEDEFAULT;
        int             y = CW_USEDEFAULT;
        int             Width = CW_USEDEFAULT;
        int             Height = CW_USEDEFAULT;
        HWND            hParent = NULL;
        HMENU           hMenu = NULL;
    };
}

#endif
