#ifndef __WindowREGISTER_H__
#define __WindowREGISTER_H__

#include "..\WinError.h"

namespace rad
{
    LRESULT CALLBACK WndHandlerWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    class RegClass : public WNDCLASSEX
    {
    public:
        RegClass(HINSTANCE _hInstance, LPCTSTR _ClassName);
        ATOM Register();

        static ATOM GetSimple(HINSTANCE _hInstance);
    };

    template <class TWND>
    class WindowCreate
    {
    public:
        WindowCreate(HINSTANCE _hInstance, LPCTSTR _WindowName)
        {
            hInstance = _hInstance;
            ClassName = MAKEINTATOM(RegClass::GetSimple(_hInstance));
            WindowName = _WindowName;
        }

        WindowCreate(HINSTANCE _hInstance, LPCTSTR _WindowName, LPCTSTR _ClassName)
        {
            hInstance = _hInstance;
            ClassName = _ClassName;
            WindowName = _WindowName;
        }

        WindowCreate(HINSTANCE _hInstance, LPCTSTR _WindowName, ATOM _Class)
        {
            hInstance = _hInstance;
            ClassName = MAKEINTATOM(_Class);
            WindowName = _WindowName;
        }

        ~WindowCreate() { }

        TWND* Create()
        {
            std::auto_ptr<TWND> WindowHandler(CreateObject());

            TWND::Create(WindowHandler.get(),
                ClassName,
                WindowName,
                Style,
                ExStyle,
                x, y,
                Width, Height,
                Parent, hMenu,
                hInstance);

            return WindowHandler.release();
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
        HWND            Parent = NULL;
        HMENU           hMenu = NULL;

    protected:
        virtual TWND* CreateObject() { return new TWND(); };
    };
}

#endif
