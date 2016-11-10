#ifndef __WindowHANDLER_H__
#define __WindowHANDLER_H__

#include "WindowMap.H"

namespace rad
{
    class DevContext;
    class PaintDC;
    class RegClass;
    class WindowCreate;

    class Window : public WindowMap
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

    public:
        void CreateWnd(const WindowCreate& wc);
        void CreateWnd(HINSTANCE hInstance, LPCTSTR WindowName, HWND hParent = NULL);

    protected:
        virtual LRESULT OnMessage(UINT Message, WPARAM wParam, LPARAM lParam);
        virtual LRESULT OnCreate(LPCREATESTRUCT CreateStruct);
        virtual LRESULT OnPaint(PaintDC& DC);
        virtual LRESULT OnMove(int x, int y);
        virtual LRESULT OnMouseMove(UINT Keys, int x, int y);
        virtual LRESULT OnMouseLeave();
        virtual LRESULT OnLeftButtonDown(UINT Keys, int x, int y);
        virtual LRESULT OnLeftButtonUp(UINT Keys, int x, int y);
        virtual LRESULT OnLeftButtonDblClk(UINT Keys, int x, int y);
        virtual LRESULT OnRightButtonDown(UINT Keys, int x, int y);
        virtual LRESULT OnRightButtonUp(UINT Keys, int x, int y);
        virtual LRESULT OnRightButtonDblClk(UINT Keys, int x, int y);
        virtual LRESULT OnMiddleButtonDown(UINT Keys, int x, int y);
        virtual LRESULT OnMiddleButtonUp(UINT Keys, int x, int y);
        virtual LRESULT OnMiddleButtonDblClk(UINT Keys, int x, int y);
        virtual LRESULT OnActivate(int option, HWND hWnd);
        virtual LRESULT OnCommand(WORD NotifyCode, WORD ID, HWND hWnd);
        virtual LRESULT OnContextMenu(HWND hWnd, int x, int y);
        virtual LRESULT OnDrawItem(UINT ID, LPDRAWITEMSTRUCT DrawItem);
        virtual LRESULT OnDropFiles(HDROP hDrop);
        virtual LRESULT OnEraseBackground(DevContext& DC);
        virtual LRESULT OnEnterSizeMove();
        virtual LRESULT OnExitSizeMove();
        virtual HICON OnGetIcon(int Type);
        virtual LRESULT OnGetMinMaxInfo(MINMAXINFO* MinMaxInfo);
        virtual LRESULT OnHotKey(UINT Modifier, UINT VKey, int Id);
        virtual LRESULT OnHScroll(HWND ScrollBar, int Code, int Pos);
        virtual LRESULT OnVScroll(HWND ScrollBar, int Code, int Pos);
        virtual LRESULT OnInitMenuPopup(HMENU hMenu, UINT Pos, BOOL SystemMenu);
        virtual LRESULT OnKeyDown(int VirtKey, const KeyInfoT* KeyInfo);
        virtual LRESULT OnKeyUp(int VirtKey, const KeyInfoT* KeyInfo);
        virtual LRESULT OnMeasureItem(UINT ID, LPMEASUREITEMSTRUCT MeasureItem);
        virtual LRESULT OnMenuSelect(UINT Item, UINT Flags, HMENU Menu);
        virtual LRESULT OnMouseWheel(int Flags, short Distance, const POINTS& Point);
        virtual LRESULT OnNotify(int CtrlID, LPNMHDR Header);
        virtual LRESULT OnSettingChange(UINT Flag, LPCTSTR Section);
        virtual LRESULT OnSetCursor(HWND hWnd, UINT HitTest, UINT MouseMsg);
        virtual LRESULT OnSetFocus(HWND hLostFocus);
        virtual LRESULT OnSize(UINT Type, int cx, int cy);
        virtual LRESULT OnSizing(int Corner, LPRECT rect);
        virtual LRESULT OnSysKeyDown(int VirtKey, const KeyInfoT* KeyInfo);
        virtual LRESULT OnSysKeyUp(int VirtKey, const KeyInfoT* KeyInfo);
        virtual LRESULT OnSysCommand(WORD ID, int x, int y);
        virtual LRESULT OnTimer(UINT ID, TIMERPROC* TimerProc);
        virtual LRESULT OnClose();
        virtual LRESULT OnDestroy();
        virtual LRESULT OnNCMouseMove(UINT HitTest, int x, int y);
        virtual LRESULT OnNCDestroy();
        virtual LRESULT UnknownMessage(UINT Message, WPARAM wParam, LPARAM lParam);

    private:
        static void SetLastMessage(WNDPROC DefWndProc, UINT Message, WPARAM wParam, LPARAM lParam)
        {
            s_DefWndProc = DefWndProc;
            s_LastMessage = Message;
            s_LastwParam = wParam;
            s_LastlParam = lParam;
        }

        static LRESULT DoDefault(HWND hWnd)
        {
            return s_DefWndProc(hWnd, s_LastMessage, s_LastwParam, s_LastlParam);
        }

    private:
        static WNDPROC s_DefWndProc;
        static UINT    s_LastMessage;
        static WPARAM  s_LastwParam;
        static LPARAM  s_LastlParam;

    protected:
        LRESULT DoDefault()
        {
            return DoDefault(GetHWND());
        }

    private:
        friend RegClass;
        static LRESULT CALLBACK WndHandlerWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, WNDPROC DefWndProc);
        static LRESULT CALLBACK DefWndHandlerWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    };
}

#endif //__WindowHANDLER_H__
