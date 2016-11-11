#ifndef __WindowCREATE_H__
#define __WindowCREATE_H__

#include "..\WinError.h"

namespace rad
{
    class WindowCreate
    {
    public:
        WindowCreate(HINSTANCE _hInstance, LPCTSTR _ClassName)
        {
            hInstance = _hInstance;
            ClassName = _ClassName;
        }

        WindowCreate(HINSTANCE _hInstance, ATOM Class)
        {
            hInstance = _hInstance;
            ClassName = MAKEINTATOM(Class);
        }

        ~WindowCreate() { }

        HWND Create(LPCTSTR WindowName, LPVOID data, HWND _hParent = NULL) const
        {
            HWND hWnd = CreateWindowEx(ExStyle,
                ClassName,
                WindowName,
                Style,
                x, y,
                Width, Height,
                _hParent, hMenu,
                hInstance,
                data);

            if (hWnd == NULL)
                ThrowWinError(_T(__FUNCTION__));

            return hWnd;
        }

        HINSTANCE       hInstance = NULL;
        LPCTSTR         ClassName = nullptr;
        DWORD           Style = WS_OVERLAPPEDWINDOW;
        DWORD           ExStyle = 0;
        int             x = CW_USEDEFAULT;
        int             y = CW_USEDEFAULT;
        int             Width = CW_USEDEFAULT;
        int             Height = CW_USEDEFAULT;
        HMENU           hMenu = NULL;
    };

    class MDIChildCreate : public MDICREATESTRUCT
    {
    public:
        MDIChildCreate(HINSTANCE hInstance, ATOM Atom)
        {
            ZeroMemory(this, sizeof(MDICREATESTRUCT));
            hOwner = hInstance;
            szClass = MAKEINTATOM(Atom);
            x = CW_USEDEFAULT;
            y = CW_USEDEFAULT;
            cx = CW_USEDEFAULT;
            cy = CW_USEDEFAULT;
            //style = MDIS_ALLCHILDSTYLES;
        }

        HWND Create(HWND hMDIClient, LPCTSTR WindowName, LPVOID data)
        {
            szTitle = WindowName;
            lParam = (LPARAM) data;
            HWND hWnd = (HWND) SendMessage(hMDIClient, WM_MDICREATE, 0, (LPARAM) this);

            if (hWnd == NULL)
                ThrowWinError(_T(__FUNCTION__));

            return hWnd;
        }
    };
}

#endif
