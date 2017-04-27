#ifndef __WindowCREATE_H__
#define __WindowCREATE_H__

#include "..\WinError.h"
#include "WindowProxy.h"

namespace rad
{
    class WindowCreate
    {
    public:
        WindowCreate(HINSTANCE _hInstance)
        {
            hInstance = _hInstance;
        }

        ~WindowCreate() { }

        HWND Create(LPCTSTR WindowName, LPVOID data, LPCTSTR _ClassName, WindowProxy _Parent = WindowProxy()) const
        {
            HWND hWnd = CreateWindowEx(ExStyle,
                _ClassName,
                WindowName,
                Style,
                x, y,
                Width, Height,
                _Parent.GetHWND(), hMenu,
                hInstance,
                data);

            if (hWnd == NULL)
                ThrowWinError(_T(__FUNCTION__));

            return hWnd;
        }

        HINSTANCE       hInstance = NULL;
        DWORD           Style = WS_OVERLAPPEDWINDOW;
        DWORD           ExStyle = 0;
        int             x = CW_USEDEFAULT;
        int             y = CW_USEDEFAULT;
        int             Width = CW_USEDEFAULT;
        int             Height = CW_USEDEFAULT;
        HMENU           hMenu = NULL;
    };

    class MDIChildCreate
    {
    public:
        MDIChildCreate(HINSTANCE _hInstance)
        {
            hInstance = _hInstance;
        }

        HWND Create(HWND hMDIClient, LPCTSTR WindowName, LPVOID data, LPCTSTR _ClassName) const
        {
            MDICREATESTRUCT mds = {};
            mds.szTitle = WindowName;
            mds.lParam = (LPARAM) data;
            mds.szClass = _ClassName;
            mds.hOwner = hInstance;
            mds.x = x;
            mds.y = y;
            mds.cx = Width;
            mds.cy = Height;
            mds.style = Style;

            HWND hWnd = (HWND) SendMessage(hMDIClient, WM_MDICREATE, 0, (LPARAM) &mds);

            if (hWnd == NULL)
                ThrowWinError(_T(__FUNCTION__));

            return hWnd;
        }

        HINSTANCE       hInstance = NULL;
        DWORD           Style = 0;  // MDIS_ALLCHILDSTYLES
        int             x = CW_USEDEFAULT;
        int             y = CW_USEDEFAULT;
        int             Width = CW_USEDEFAULT;
        int             Height = CW_USEDEFAULT;
    };
}

#endif
