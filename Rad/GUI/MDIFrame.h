#ifndef __MDIFrameHANDLER_H__
#define __MDIFrameHANDLER_H__

#include "Window.h"

namespace rad
{
    class MDIFrame : public Window
    {
    public:
        virtual RegClass GetMDIFrameReg(HINSTANCE _hInstance);
        ATOM GetMDIFrameAtom(HINSTANCE hInstance);
        virtual RegClass GetMDIChildReg(HINSTANCE _hInstance);
        ATOM GetMDIChildAtom(HINSTANCE hInstance);

    public:
        LPCTSTR GetWndClassName(HINSTANCE hInstance) override;
        virtual LPCTSTR GetMDIChildClassName(HINSTANCE hInstance);

        Window* CreateChild(Window* w, LPCTSTR WindowName);

    protected:
        virtual LRESULT OnCreate(LPCREATESTRUCT CreateStruct) override
        {
            CLIENTCREATESTRUCT ccs = GetClientCreate();
            ::CreateWindow(_T("MDICLIENT"), (LPCTSTR) NULL,
                WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL,
                0, 0, 0, 0, GetHWND(), (HMENU) 0xCAC, CreateStruct->hInstance, (LPSTR) &ccs);

            return Window::OnCreate(CreateStruct);
        }

    public:
        WindowProxy GetMDIClient()
        {
            return GetMDIClient(GetHWND());
        }

    private:
        static HWND GetMDIClient(HWND hFrame)
        {
            return FindWindowEx(hFrame, NULL, _T("MDICLIENT"), nullptr);;
        }

    public:
        virtual CLIENTCREATESTRUCT GetClientCreate()
        {
            CLIENTCREATESTRUCT ccs = {};
            // Override and fill in struct
            return ccs;
        }

    protected:
        static LRESULT CALLBACK MDIFrameWndHandlerWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            return WndHandlerWindowProc(hWnd, uMsg, wParam, lParam, DefMDIFrameProc);
        }

        static LRESULT CALLBACK DefMDIFrameProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            HWND hMDIClient = GetMDIClient(hWnd);
            return DefFrameProc(hWnd, hMDIClient, uMsg, wParam, lParam);
        }

        static LRESULT CALLBACK MDIChildWndHandlerWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            return WndHandlerWindowProc(hWnd, uMsg, wParam, lParam, DefMDIChildProc);
        }
    };
}

#endif //__MDIFrameHANDLER_H__
