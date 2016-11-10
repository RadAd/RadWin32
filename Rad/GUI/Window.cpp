#include "Window.H"

#include <WindowsX.H>
#include <algorithm>

#include "DevContext.H"
#include "WindowCreate.h"
#include "WindowListener.h"
//#include "..\..\Logging.H"

namespace rad
{
    const HWND WindowProxy::Invalid = NULL;
    const HDC DevContext::Invalid = NULL;

    Window::HWNDMapT Window::s_HWNDMap;
    int Window::s_ExitCode = 0;

    LRESULT CALLBACK WndHandlerWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        static int depth = 0;
        LRESULT RetVal = 0;

        ++depth;
        try
        {
            //LogString("WndHandlerWindowProc: ");
            //LogMessage(hWnd, uMsg, wParam, lParam);

            Window* WindowHandler = Window::FromHWND(hWnd);

            if (uMsg == WM_NCCREATE)
            {
                assert(WindowHandler == nullptr);
                WindowHandler = (Window*) (((LPCREATESTRUCT) lParam)->lpCreateParams);
                WindowHandler->AttachMap(hWnd);
            }

            if (WindowHandler != nullptr)
            {
                RetVal = WindowHandler->OnMessage(uMsg, wParam, lParam);

                if (!IsWindow(hWnd) && depth <= 1)
                {
                    WindowHandler->DetachMap();
                    //delete WindowHandler;
                }
            }
            else
            {
#ifdef    _DEBUG
                switch (uMsg)
                {
                case WM_GETMINMAXINFO:
                    RetVal = DefWindowProc(hWnd, uMsg, wParam, lParam);
                    break;

                default:
                    // Should never get here
                    RetVal = DefWindowProc(hWnd, uMsg, wParam, lParam);
                    break;
                }
#else
                RetVal = DefWindowProc(hWnd, uMsg, wParam, lParam);
#endif
            }
        }
        catch (const WinError& e)
        {
            TCHAR    Text[1024];
            GetWindowText(hWnd, Text, std::extent<decltype(Text)>::value);
            MessageBox(hWnd, e.GetString().c_str(), Text, MB_OK | MB_ICONSTOP);
        }
        catch (...)
        {
            TCHAR    Text[1024];
            GetWindowText(hWnd, Text, std::extent<decltype(Text)>::value);
            MessageBox(hWnd, _T("Unknown Exception. Caught in ") _T(__FUNCTION__), Text, MB_OK | MB_ICONSTOP);
        }
        --depth;

        return RetVal;
    }

    Window::Window(const WindowCreate& wc, WNDPROC DefWndProc)
        : m_DefWndProc(DefWndProc)
    {
        assert(m_DefWndProc != nullptr);
        wc.Create((LPVOID) this);
    }

    Window::Window(HINSTANCE hInstance, LPCTSTR WindowName, WNDPROC DefWndProc)
        : m_DefWndProc(DefWndProc)
    {
        assert(m_DefWndProc != nullptr);
        WindowCreate wc(hInstance, WindowName);
        wc.Create((LPVOID) this);
    }

    Window* Window::FromHWND(HWND hWnd)
    {
        HWNDMapT::const_iterator    it = s_HWNDMap.find(hWnd);

        if (it == s_HWNDMap.end())
            return nullptr;
        else
            return it->second;
    }

    void Window::AttachMap(HWND hWnd)
    {
        Attach(hWnd);
        s_HWNDMap.insert(HWNDMapT::value_type(GetHWND(), this));
    }

    void Window::DetachMap()
    {
        s_HWNDMap.erase(GetHWND());
        if (s_HWNDMap.size() == 0)        // Last window has been destroyed
            PostQuitMessage(s_ExitCode);
        delete this;
    }

    LRESULT Window::OnMessage(UINT Message, WPARAM wParam, LPARAM lParam)
    {
        LRESULT        RetVal = 0;

        try
        {
            SetLastMessage(Message, wParam, lParam);

            for (WindowListener* pListener : m_WindowListeners)
                pListener->OnPreMessage(this, Message, wParam, lParam);

            switch (Message)
            {
            case WM_CREATE:
                RetVal = OnCreate((LPCREATESTRUCT) lParam);
                break;

            case WM_PAINT:
                {
                    PaintDC DC(*this);
                    RetVal = OnPaint(DC);
                }
                break;

            case WM_MOVE:
                RetVal = OnMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                break;

            case WM_MOUSEMOVE:
                RetVal = OnMouseMove((UINT) wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                break;

            case WM_MOUSELEAVE:
                RetVal = OnMouseLeave();
                break;

            case WM_LBUTTONDOWN:
                RetVal = OnLeftButtonDown((UINT) wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                break;

            case WM_LBUTTONUP:
                RetVal = OnLeftButtonUp((UINT) wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                break;

            case WM_LBUTTONDBLCLK:
                RetVal = OnLeftButtonDblClk((UINT) wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                break;

            case WM_RBUTTONDOWN:
                RetVal = OnRightButtonDown((UINT) wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                break;

            case WM_RBUTTONUP:
                RetVal = OnRightButtonUp((UINT) wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                break;

            case WM_RBUTTONDBLCLK:
                RetVal = OnRightButtonDblClk((UINT) wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                break;

            case WM_MBUTTONDOWN:
                RetVal = OnMiddleButtonDown((UINT) wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                break;

            case WM_MBUTTONUP:
                RetVal = OnMiddleButtonUp((UINT) wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                break;

            case WM_MBUTTONDBLCLK:
                RetVal = OnMiddleButtonDblClk((UINT) wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                break;

            case WM_SYSCOMMAND:
                RetVal = OnSysCommand((WORD) wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                break;

            case WM_ACTIVATE:
                RetVal = OnActivate((int) wParam, (HWND) lParam);
                break;

            case WM_COMMAND:
                RetVal = OnCommand(HIWORD(wParam), LOWORD(wParam), (HWND) lParam);
                break;

            case WM_CONTEXTMENU:
                RetVal = OnContextMenu((HWND) wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                break;

            case WM_DRAWITEM:
                RetVal = OnDrawItem((UINT) wParam, (LPDRAWITEMSTRUCT) lParam);
                break;

            case WM_DROPFILES:
                RetVal = OnDropFiles((HDROP) wParam);
                break;

            case WM_ERASEBKGND:
                {
                    DevContext dc((HDC) wParam);
                    RetVal = OnEraseBackground(dc);
                }
                break;

            case WM_ENTERSIZEMOVE:
                RetVal = (LRESULT) OnEnterSizeMove();
                break;

            case WM_EXITSIZEMOVE:
                RetVal = (LRESULT) OnExitSizeMove();
                break;

            case WM_GETICON:
                RetVal = (LRESULT) OnGetIcon((int) wParam);
                break;

            case WM_GETMINMAXINFO:
                RetVal = OnGetMinMaxInfo((LPMINMAXINFO) lParam);
                break;

            case WM_HOTKEY:
                RetVal = OnHotKey(LOWORD(lParam), HIWORD(lParam), (int) wParam);
                break;

            case WM_HSCROLL:
                RetVal = OnHScroll((HWND) lParam, LOWORD(wParam), HIWORD(wParam));
                break;

            case WM_VSCROLL:
                RetVal = OnVScroll((HWND) lParam, LOWORD(wParam), HIWORD(wParam));
                break;

            case WM_INITMENUPOPUP:
                RetVal = OnInitMenuPopup((HMENU) wParam, (UINT) LOWORD(lParam), (BOOL) HIWORD(lParam));
                break;

            case WM_KEYDOWN:
                RetVal = OnKeyDown((int) wParam, (KeyInfoT*) &lParam);
                break;

            case WM_KEYUP:
                RetVal = OnKeyUp((int) wParam, (KeyInfoT*) &lParam);
                break;

            case WM_SETFOCUS:
                RetVal = OnSetFocus((HWND) wParam);
                break;

            case WM_SYSKEYDOWN:
                RetVal = OnSysKeyDown((int) wParam, (KeyInfoT*) &lParam);
                break;

            case WM_SYSKEYUP:
                RetVal = OnSysKeyUp((int) wParam, (KeyInfoT*) &lParam);
                break;

            case WM_MEASUREITEM:
                RetVal = OnMeasureItem((int) wParam, (LPMEASUREITEMSTRUCT) lParam);
                break;

            case WM_MENUSELECT:
                RetVal = OnMenuSelect((UINT) LOWORD(wParam), (UINT) HIWORD(wParam), (HMENU) lParam);
                break;

            case WM_MOUSEWHEEL:
                RetVal = OnMouseWheel((int) LOWORD(wParam), (short) HIWORD(wParam), MAKEPOINTS(lParam));
                break;

            case WM_NOTIFY:
                RetVal = OnNotify((int) wParam, (LPNMHDR) lParam);
                break;

            case WM_SETTINGCHANGE:
                RetVal = OnSettingChange((UINT) wParam, (LPCTSTR) lParam);
                break;

            case WM_SETCURSOR:
                RetVal = OnSetCursor((HWND) wParam, (UINT) LOWORD(lParam), (UINT) HIWORD(lParam));
                break;

            case WM_SIZE:
                RetVal = OnSize((UINT) wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                break;

            case WM_SIZING:
                RetVal = OnSizing((int) wParam, (LPRECT) lParam);
                break;

            case WM_TIMER:
                RetVal = OnTimer((UINT) wParam, (TIMERPROC*) lParam);
                break;

            case WM_CLOSE:
                RetVal = OnClose();
                break;

            case WM_DESTROY:
                RetVal = OnDestroy();
                break;

            case WM_NCMOUSEMOVE:
                RetVal = OnNCMouseMove((UINT) wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                break;

            case WM_NCDESTROY:
                RetVal = OnNCDestroy();
                break;

            default:
                RetVal = UnknownMessage(Message, wParam, lParam);
                break;
            }

            //if (Message != WM_NCDESTROY)    // the object is destroyed with this message
            {
                for (WindowListener* pListener : m_WindowListeners)
                    pListener->OnPostMessage(this, Message, wParam, lParam);
            }
        }
        catch (const WinError& e)
        {
            TCHAR    Text[1024];
            GetWindowText(Text);
            MessageBox(*this, e.GetString().c_str(), Text, MB_OK | MB_ICONSTOP);
        }
        catch (...)
        {
            TCHAR    Text[1024];
            GetWindowText(Text);
            MessageBox(*this, _T("Unknown Exception. Caught in ") _T(__FUNCTION__), Text, MB_OK | MB_ICONSTOP);
        }

        return RetVal;
    }

    LRESULT Window::DoDefault()
    {
        try
        {
            return m_DefWndProc(GetHWND(), m_LastMessage, m_LastwParam, m_LastlParam);
        }
        catch (const WinError& e)
        {
            TCHAR    Text[1024];
            GetWindowText(Text);
            MessageBox(*this, e.GetString().c_str(), Text, MB_OK | MB_ICONSTOP);
        }
        catch (...)
        {
            TCHAR    Text[1024];
            GetWindowText(Text);
            MessageBox(*this, _T("Unknown Exception. Caught in ") _T(__FUNCTION__), Text, MB_OK | MB_ICONSTOP);
        }
        return 0;
    }

    LRESULT Window::OnCreate(LPCREATESTRUCT /*CreateStruct*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnPaint(PaintDC& /*DC*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnMove(int /*x*/, int /*y*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnMouseMove(UINT /*Keys*/, int /*x*/, int /*y*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnMouseLeave()
    {
        return DoDefault();
    }

    LRESULT Window::OnLeftButtonDown(UINT /*Keys*/, int /*x*/, int /*y*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnLeftButtonUp(UINT /*Keys*/, int /*x*/, int /*y*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnLeftButtonDblClk(UINT /*Keys*/, int /*x*/, int /*y*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnRightButtonDown(UINT /*Keys*/, int /*x*/, int /*y*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnRightButtonUp(UINT /*Keys*/, int /*x*/, int /*y*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnRightButtonDblClk(UINT /*Keys*/, int /*x*/, int /*y*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnMiddleButtonDown(UINT /*Keys*/, int /*x*/, int /*y*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnMiddleButtonUp(UINT /*Keys*/, int /*x*/, int /*y*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnMiddleButtonDblClk(UINT /*Keys*/, int /*x*/, int /*y*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnActivate(int /*option*/, HWND /*hWnd*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnCommand(WORD /*NotifyCode*/, WORD /*ID*/, HWND /*hWnd*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnContextMenu(HWND /*hWnd*/, int /*x*/, int /*y*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnDrawItem(UINT /*ID*/, LPDRAWITEMSTRUCT /*DrawItem*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnDropFiles(HDROP hDrop)
    {
        DragFinish(hDrop);
        return DoDefault();
    }

    LRESULT Window::OnEraseBackground(DevContext& /*DC*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnEnterSizeMove()
    {
        return DoDefault();
    }

    LRESULT Window::OnExitSizeMove()
    {
        return DoDefault();
    }

    HICON Window::OnGetIcon(int /*Type*/)
    {
        return (HICON) DoDefault();
    }

    LRESULT Window::OnGetMinMaxInfo(MINMAXINFO* /*MinMaxInfo*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnHotKey(UINT /*Modifier*/, UINT /*VKey*/, int /*Id*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnHScroll(HWND /*ScrollBar*/, int /*Code*/, int /*Pos*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnVScroll(HWND /*ScrollBar*/, int /*Code*/, int /*Pos*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnInitMenuPopup(HMENU /*hMenu*/, UINT /*Pos*/, BOOL /*SystemMenu*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnKeyDown(int /*VirtKey*/, const KeyInfoT* /*KeyInfo*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnKeyUp(int /*VirtKey*/, const KeyInfoT* /*KeyInfo*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnMeasureItem(UINT /*ID*/, LPMEASUREITEMSTRUCT /*MeasureItem*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnMenuSelect(UINT /*Item*/, UINT /*Flags*/, HMENU /*Menu*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnMouseWheel(int /*Flags*/, short /*Distance*/, const POINTS& /*Point*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnNotify(int /*CtrlID*/, LPNMHDR /*Header*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnSettingChange(UINT /*Flag*/, LPCTSTR /*Section*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnSetCursor(HWND /*hWnd*/, UINT /*HitTest*/, UINT /*MouseMsg*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnSetFocus(HWND /*hLostFocus*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnSize(UINT /*Type*/, int /*cx*/, int /*cy*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnSizing(int /*Corner*/, LPRECT /*rect*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnSysKeyDown(int /*VirtKey*/, const KeyInfoT* /*KeyInfo*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnSysKeyUp(int /*VirtKey*/, const KeyInfoT* /*KeyInfo*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnSysCommand(WORD /*ID*/, int /*x*/, int /*y*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnTimer(UINT /*ID*/, TIMERPROC* /*TimerProc*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnClose()
    {
        return DoDefault();
    }

    LRESULT Window::OnDestroy()
    {
        return DoDefault();
    }

    LRESULT Window::OnNCMouseMove(UINT /*HitTest*/, int /*x*/, int /*y*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnNCDestroy()
    {
        return DoDefault();
    }

    LRESULT Window::UnknownMessage(UINT /*Message*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
    {
        return DoDefault();
    }

    void Window::RemoveWindowListener(WindowListener* pWindowListener)
    {
        WindowListenersContT::iterator it = std::find(m_WindowListeners.begin(), m_WindowListeners.end(), pWindowListener);
        if (it != m_WindowListeners.end())
            m_WindowListeners.erase(it);
    }
}
