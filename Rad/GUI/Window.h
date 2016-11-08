#ifndef __WindowHANDLER_H__
#define __WindowHANDLER_H__

#include "WindowProxy.H"

#include <map>
#include <vector>

namespace rad
{
    class DevContext;
    class CPaintDC;
    class WindowListener;

    class Window : public WindowProxy
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

    protected:
        Window(WNDPROC DefWndProc = DefWindowProc)
            : m_DefWndProc(DefWndProc)
        {
            assert(m_DefWndProc != nullptr);
        }
        ~Window()
        {
        }

    protected:
        UINT GetLastMessage() const { return m_LastMessage; }
        WPARAM GetLastwParam() const { return m_LastwParam; }
        LPARAM GetLastlParam() const { return m_LastlParam; }

    public:
        void AddWindowListener(WindowListener* pWindowListener)
        {
            m_WindowListeners.push_back(pWindowListener);
        }

        void RemoveWindowListener(WindowListener* pWindowListener);

    private:
        typedef std::vector<WindowListener*> WindowListenersContT;
        WindowListenersContT    m_WindowListeners;

    public:
        virtual LRESULT OnMessage(UINT Message, WPARAM wParam, LPARAM lParam);
        virtual LRESULT OnCreate(LPCREATESTRUCT CreateStruct);
        virtual LRESULT OnPaint(CPaintDC &DC);
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
        virtual LRESULT OnEraseBackground(DevContext &DC);
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
        virtual LRESULT OnMouseWheel(int Flags, short Distance, const POINTS &Point);
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

    protected:
        LRESULT DoDefault();

    private:
        void SetLastMessage(UINT Message, WPARAM wParam, LPARAM lParam)
        {
            m_LastMessage = Message;
            m_LastwParam = wParam;
            m_LastlParam = lParam;
        }

        UINT    m_LastMessage;
        WPARAM  m_LastwParam;
        LPARAM  m_LastlParam;
        WNDPROC m_DefWndProc;

    public:    // static
        static HWND Create(Window* WindowHandler, LPCTSTR ClassName, LPCTSTR WindowName, DWORD Style, DWORD ExStyle,
            int x, int y, int Width, int Height,
            HWND hParent, HMENU hMenu, HINSTANCE hInstance);
        static Window* FromHWND(HWND hWnd);
        void AttachMap(HWND hWnd);
        void DetachMap();

    private:
        typedef std::map<HWND, Window*> HWNDMapT;
        static HWNDMapT     s_HWNDMap;
        static int          s_ExitCode;
    };
}

#endif //__WindowHANDLER_H__
