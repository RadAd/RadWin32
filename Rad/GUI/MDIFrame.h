#ifndef __MDIFrameHANDLER_H__
#define __MDIFrameHANDLER_H__

#include "Window.h"
#include "WindowCreate.h"

namespace rad
{
    class MDIFrame : public Window
    {
    public:
        using Window::CreateWnd;

        virtual void CreateWnd(HINSTANCE hInstance, LPCTSTR WindowName, HWND hParent = NULL)
        {
            CreateWnd(WindowCreate::GetMDIFrame(hInstance), WindowName, hParent);
        }

    protected:
        virtual LRESULT OnCreate(LPCREATESTRUCT CreateStruct)
        {
            CLIENTCREATESTRUCT ccs = GetClientCreate();
            ::CreateWindow(_T("MDICLIENT"), (LPCTSTR) NULL,
                WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL,
                0, 0, 0, 0, GetHWND(), (HMENU) 0xCAC, CreateStruct->hInstance, (LPSTR) &ccs);

            return Window::OnCreate(CreateStruct);
        }

    public:
        Window* CreateChild(Window* w, LPCTSTR WindowName)
        {
            HINSTANCE hInstance = (HINSTANCE) GetWindowLongPtr(GWLP_HINSTANCE);

            MDIChildCreate c(hInstance);
            c.Create(GetMDIClient().GetHWND(), WindowName, w);
            return w;
        }

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
        friend RegClass;
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
