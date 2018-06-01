#ifndef __WindowHANDLER_H__
#define __WindowHANDLER_H__

#include "WindowMap.h"
#include "WindowMessage.h"

namespace rad
{
    class DevContextRef;
    class MDIChildCreate;
    class MDIFrame;
    class PaintDC;
    class RegClass;
    class WindowCreate;

    class Window : public WindowDelete, private WindowMessage
    {
    public:
        struct KeyInfoT
        {
            int Repeat : 16,
                ScanCode : 8,
                Extended : 1,
                Reserved : 4,
                Context : 1,
                Previous : 1,
                Transition : 1;
        };

        static RegClass GetSimpleReg(HINSTANCE hInstance);
        static ATOM GetSimpleAtom(HINSTANCE hInstance);

    protected:
        using WindowDelete::WindowDelete;
        using WindowMessage::Add;

    public:
        virtual LPCTSTR GetWndClassName(HINSTANCE hInstance);
        virtual LPCTSTR GetMDIChildClassName(HINSTANCE hInstance);

        void CreateWnd(const WindowCreate& wc, LPCTSTR WindowName, HWND hParent = NULL);
        void CreateWnd(HINSTANCE hInstance, LPCTSTR WindowName, HWND hParent = NULL);
        void CreateWnd(LPCTSTR WindowName, HWND hParent);
        void CreateMDIChildWnd(MDIChildCreate& wc, LPCTSTR WindowName, HWND hMDIClient);
        void CreateMDIChildWnd(LPCTSTR WindowName, HWND hMDIClient);
        void CreateMDIChildWnd(LPCTSTR WindowName, MDIFrame* f);

    protected:
        virtual LRESULT OnMessage(UINT Message, WPARAM wParam, LPARAM lParam);
        virtual LRESULT OnCreate(LPCREATESTRUCT CreateStruct);
        virtual LRESULT OnPaint(PaintDC& DC);
        virtual LRESULT OnMove(POINTS Point);
        virtual LRESULT OnMouseMove(UINT Keys, POINTS Point);
        virtual LRESULT OnMouseLeave();
        virtual LRESULT OnLeftButtonDown(UINT Keys, POINTS Point);
        virtual LRESULT OnLeftButtonUp(UINT Keys, POINTS Point);
        virtual LRESULT OnLeftButtonDblClk(UINT Keys, POINTS Point);
        virtual LRESULT OnRightButtonDown(UINT Keys, POINTS Point);
        virtual LRESULT OnRightButtonUp(UINT Keys, POINTS Point);
        virtual LRESULT OnRightButtonDblClk(UINT Keys, POINTS Point);
        virtual LRESULT OnMiddleButtonDown(UINT Keys, POINTS Point);
        virtual LRESULT OnMiddleButtonUp(UINT Keys, POINTS Point);
        virtual LRESULT OnMiddleButtonDblClk(UINT Keys, POINTS Point);
        virtual LRESULT OnActivate(int option, HWND hWnd);
        virtual LRESULT OnCommand(WORD NotifyCode, WORD ID, HWND hWnd);
        virtual LRESULT OnContextMenu(HWND hWnd, POINTS Point);
        virtual LRESULT OnDrawItem(UINT ID, LPDRAWITEMSTRUCT DrawItem);
        virtual LRESULT OnDropFiles(HDROP hDrop);
        virtual LRESULT OnEraseBackground(DevContextRef DC);
        virtual LRESULT OnEnterSizeMove();
        virtual LRESULT OnExitSizeMove();
        virtual HICON OnGetIcon(int Type);
        virtual LRESULT OnGetMinMaxInfo(MINMAXINFO* MinMaxInfo);
        virtual LRESULT OnHotKey(UINT Modifier, UINT VKey, int Id);
        virtual LRESULT OnHScroll(HWND ScrollBar, int Code, int Pos);
        virtual LRESULT OnVScroll(HWND ScrollBar, int Code, int Pos);
        virtual LRESULT OnInitMenuPopup(HMENU hMenu, UINT Pos, BOOL SystemMenu);
        virtual LRESULT OnKeyDown(int VirtKey, KeyInfoT KeyInfo);
        virtual LRESULT OnKeyUp(int VirtKey, KeyInfoT KeyInfo);
        virtual LRESULT OnMeasureItem(UINT ID, LPMEASUREITEMSTRUCT MeasureItem);
        virtual LRESULT OnMenuSelect(UINT Item, UINT Flags, HMENU Menu);
        virtual LRESULT OnMouseWheel(int Flags, short Distance, POINTS Point);
        virtual LRESULT OnNotify(int CtrlID, LPNMHDR Header);
        virtual LRESULT OnSettingChange(UINT Flag, LPCTSTR Section);
        virtual LRESULT OnSetCursor(HWND hWnd, UINT HitTest, UINT MouseMsg);
        virtual LRESULT OnSetFocus(HWND hLostFocus);
        virtual LRESULT OnSize(UINT Type, int cx, int cy);
        virtual LRESULT OnSizing(int Corner, LPRECT rect);
        virtual LRESULT OnSysKeyDown(int VirtKey, KeyInfoT KeyInfo);
        virtual LRESULT OnSysKeyUp(int VirtKey, KeyInfoT KeyInfo);
        virtual LRESULT OnSysCommand(WORD ID, POINTS Point);
        virtual LRESULT OnTimer(UINT ID, TIMERPROC* TimerProc);
        virtual LRESULT OnClose();
        virtual LRESULT OnDestroy();
        virtual LRESULT OnNCMouseMove(UINT HitTest, POINTS Point);
        virtual LRESULT OnNCDestroy();
        virtual LRESULT UnknownMessage(UINT Message, WPARAM wParam, LPARAM lParam);

    protected:
        LRESULT DoDefault()
        {
            return DoChain(this);
        }

    protected:
        static LRESULT WndHandlerWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, WNDPROC DefWndProc);
        static LRESULT CALLBACK DefWndHandlerWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    };
}

#endif //__WindowHANDLER_H__
