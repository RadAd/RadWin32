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
        virtual LRESULT OnCreate(LPCREATESTRUCT CreateStruct) override;

    public:
        WindowProxy GetMDIClient()
        {
            return GetMDIClient(GetHWND());
        }

        void CascadeWindows(UINT wHow = MDITILE_SKIPDISABLED | MDITILE_ZORDER)
        {
#if 0
            if (::CascadeWindows(GetMDIClient().GetHWND(), wHow, nullptr, 0, nullptr) == 0)
                ThrowWinError(_T(__FUNCTION__));
#else
            if (GetMDIClient().SendMessage(WM_MDICASCADE, wHow) == 0)
                ThrowWinError(_T(__FUNCTION__));
#endif
        }

        void TileWindows(UINT wHow = MDITILE_SKIPDISABLED /* | MDITILE_HORIZONTAL | MDITILE_VERTICAL */)
        {
#if 0
            if (::TileWindows(GetMDIClient().GetHWND(), wHow, nullptr, 0, nullptr) == 0)
                ThrowWinError(_T(__FUNCTION__));
#else
            if (GetMDIClient().SendMessage(WM_MDITILE, wHow) == 0)
                ThrowWinError(_T(__FUNCTION__));
#endif
        }

    private:
        static HWND GetMDIClient(HWND hFrame)
        {
            return FindWindowEx(hFrame, NULL, _T("MDICLIENT"), nullptr);;
        }

    protected:
        virtual CLIENTCREATESTRUCT GetClientCreate()
        {
            CLIENTCREATESTRUCT ccs = {};
            // Override and fill in struct
            ccs.hWindowMenu = FindWindowMenu();
            return ccs;
        }

        virtual HMENU FindWindowMenu();

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
