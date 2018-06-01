#include "Window.H"

#include <WindowsX.H>

#include "DevContext.H"
#include "MDIFrame.h"
#include "RegClass.h"
#include "WindowCreate.h"
//#include "..\..\Logging.H"

namespace rad
{
    const HDC DevContextRef::Invalid = NULL;

    RegClass Window::GetSimpleReg(HINSTANCE hInstance)
    {
        RegClass rc(hInstance, _T("SimpleWindow"), DefWndHandlerWindowProc);
        return rc;
    }

    ATOM Window::GetSimpleAtom(HINSTANCE hInstance)
    {
        static ATOM Atom = GetSimpleReg(hInstance).Register();
        return Atom;
    }

    LRESULT Window::WndHandlerWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, WNDPROC DefWndProc)
    {
        LRESULT RetVal = 0;

        try
        {
            //LogString("WndHandlerWindowProc: ");
            //LogMessage(hWnd, uMsg, wParam, lParam);

            Window* WindowHandler = dynamic_cast<Window*>(WindowMap::GetInstance()->FromHWND(hWnd));

            if (uMsg == WM_NCCREATE)
            {
                assert(WindowHandler == nullptr);
                LPCREATESTRUCT cs = (LPCREATESTRUCT) lParam;
                if (cs->dwExStyle & WS_EX_MDICHILD)
                {
                    LPMDICREATESTRUCT mcs = (LPMDICREATESTRUCT) cs->lpCreateParams;
                    WindowHandler = (Window*) mcs->lParam;
                }
                else
                    WindowHandler = (Window*) cs->lpCreateParams;
                if (WindowHandler != nullptr)
                {
                    WindowHandler->Attach(hWnd);
                    WindowMap::GetInstance()->AttachMap(WindowHandler);
                }
            }

            if (WindowHandler != nullptr)
            {
                WindowHandler->PushWndProcDepth();
                RetVal = WindowHandler->HandleMessage(uMsg, wParam, lParam, DefWndProc);
                WindowHandler->PopWndProcDepth(); // TODO Make exception safe
            }
            else
            {
#ifdef    _DEBUG
                switch (uMsg)
                {
                case WM_GETMINMAXINFO:
                    RetVal = DefWndProc(hWnd, uMsg, wParam, lParam);
                    break;

                default:
                    // Should never get here
                    RetVal = DefWndProc(hWnd, uMsg, wParam, lParam);
                    break;
                }
#else
                RetVal = DefWndProc(hWnd, uMsg, wParam, lParam);
#endif
            }
        }
        catch (const WinError& e)
        {
            TCHAR    Text[1024];
            ::GetWindowText(hWnd, Text, std::extent<decltype(Text)>::value);
            MessageBox(hWnd, e.GetString().c_str(), Text, MB_OK | MB_ICONSTOP);
        }
        catch (...)
        {
            TCHAR    Text[1024];
            ::GetWindowText(hWnd, Text, std::extent<decltype(Text)>::value);
            MessageBox(hWnd, _T("Unknown Exception. Caught in ") _T(__FUNCTION__), Text, MB_OK | MB_ICONSTOP);
        }

        return RetVal;
    }

    LRESULT Window::DefWndHandlerWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        return WndHandlerWindowProc(hWnd, uMsg, wParam, lParam, DefWindowProc);
    }

    LPCTSTR Window::GetWndClassName(HINSTANCE hInstance)
    {
        return MAKEINTATOM(GetSimpleAtom(hInstance));
    }

    LPCTSTR Window::GetMDIChildClassName(HINSTANCE hInstance)
    {
        return MAKEINTATOM(MDIFrame::GetMDIChildAtom(hInstance));
    }

    void Window::CreateWnd(const WindowCreate& wc, LPCTSTR WindowName, HWND hParent)
    {
        LPCTSTR _ClassName = GetWndClassName(wc.hInstance);
        wc.Create(WindowName, (LPVOID) this, _ClassName, hParent);
    }

    void Window::CreateWnd(HINSTANCE hInstance, LPCTSTR WindowName, HWND hParent)
    {
        CreateWnd(WindowCreate(hInstance), WindowName, hParent);
    }

    void Window::CreateWnd(LPCTSTR WindowName, HWND hParent)
    {
        assert(::IsWindow(hParent));
        HINSTANCE hInstance = (HINSTANCE) ::GetWindowLongPtr(hParent, GWLP_HINSTANCE);
        CreateWnd(hInstance, WindowName, hParent);
    }

    void Window::CreateMDIChildWnd(MDIChildCreate& wc, LPCTSTR WindowName, HWND hMDIClient)
    {
        wc.Create(hMDIClient, WindowName, this, GetMDIChildClassName(wc.hInstance));
    }

    void Window::CreateMDIChildWnd(LPCTSTR WindowName, HWND hMDIClient)
    {
        assert(::IsWindow(hMDIClient));
        HINSTANCE hInstance = (HINSTANCE) ::GetWindowLongPtr(hMDIClient, GWLP_HINSTANCE);
        MDIChildCreate wc(hInstance);
        CreateMDIChildWnd(wc, WindowName, hMDIClient);
    }

    void Window::CreateMDIChildWnd(LPCTSTR WindowName, MDIFrame* f)
    {
        CreateMDIChildWnd(WindowName, f->GetMDIClient().GetHWND());
    }

    LRESULT Window::OnMessage(UINT Message, WPARAM wParam, LPARAM lParam)
    {
        try
        {
            switch (Message)
            {
            case WM_CREATE:         return OnCreate((LPCREATESTRUCT) lParam);
            case WM_PAINT:          { PaintDC DC(*this); return OnPaint(DC); }
            case WM_MOVE:           return OnMove(MAKEPOINTS(lParam));
            case WM_MOUSEMOVE:      return OnMouseMove((UINT) wParam, MAKEPOINTS(lParam));
            case WM_MOUSELEAVE:     return OnMouseLeave();
            case WM_LBUTTONDOWN:    return OnLeftButtonDown((UINT) wParam, MAKEPOINTS(lParam));
            case WM_LBUTTONUP:      return OnLeftButtonUp((UINT) wParam, MAKEPOINTS(lParam));
            case WM_LBUTTONDBLCLK:  return OnLeftButtonDblClk((UINT) wParam, MAKEPOINTS(lParam));
            case WM_RBUTTONDOWN:    return OnRightButtonDown((UINT) wParam, MAKEPOINTS(lParam));
            case WM_RBUTTONUP:      return OnRightButtonUp((UINT) wParam, MAKEPOINTS(lParam));
            case WM_RBUTTONDBLCLK:  return OnRightButtonDblClk((UINT) wParam, MAKEPOINTS(lParam));
            case WM_MBUTTONDOWN:    return OnMiddleButtonDown((UINT) wParam, MAKEPOINTS(lParam));
            case WM_MBUTTONUP:      return OnMiddleButtonUp((UINT) wParam, MAKEPOINTS(lParam));
            case WM_MBUTTONDBLCLK:  return OnMiddleButtonDblClk((UINT) wParam, MAKEPOINTS(lParam));
            case WM_SYSCOMMAND:     return OnSysCommand((WORD) wParam, MAKEPOINTS(lParam));
            case WM_ACTIVATE:       return OnActivate((int) wParam, (HWND) lParam);
            case WM_COMMAND:        return OnCommand(HIWORD(wParam), LOWORD(wParam), (HWND) lParam);
            case WM_CONTEXTMENU:    return OnContextMenu((HWND) wParam, MAKEPOINTS(lParam));
            case WM_DRAWITEM:       return OnDrawItem((UINT) wParam, (LPDRAWITEMSTRUCT) lParam);
            case WM_DROPFILES:      return OnDropFiles((HDROP) wParam);
            case WM_ERASEBKGND:     return OnEraseBackground((HDC) wParam);
            case WM_ENTERSIZEMOVE:  return (LRESULT) OnEnterSizeMove();
            case WM_EXITSIZEMOVE:   return (LRESULT) OnExitSizeMove();
            case WM_GETICON:        return (LRESULT) OnGetIcon((int) wParam);
            case WM_GETMINMAXINFO:  return OnGetMinMaxInfo((LPMINMAXINFO) lParam);
            case WM_HOTKEY:         return OnHotKey(LOWORD(lParam), HIWORD(lParam), (int) wParam);
            case WM_HSCROLL:        return OnHScroll((HWND) lParam, LOWORD(wParam), HIWORD(wParam));
            case WM_VSCROLL:        return OnVScroll((HWND) lParam, LOWORD(wParam), HIWORD(wParam));
            case WM_INITMENUPOPUP:  return OnInitMenuPopup((HMENU) wParam, (UINT) LOWORD(lParam), (BOOL) HIWORD(lParam));
            case WM_KEYDOWN:        return OnKeyDown((int) wParam, *reinterpret_cast<KeyInfoT*>(&lParam));
            case WM_KEYUP:          return OnKeyUp((int) wParam, *reinterpret_cast<KeyInfoT*>(&lParam));
            case WM_SETFOCUS:       return OnSetFocus((HWND) wParam);
            case WM_SYSKEYDOWN:     return OnSysKeyDown((int) wParam, *reinterpret_cast<KeyInfoT*>(&lParam));
            case WM_SYSKEYUP:       return OnSysKeyUp((int) wParam, *reinterpret_cast<KeyInfoT*>(&lParam));
            case WM_MEASUREITEM:    return OnMeasureItem((int) wParam, (LPMEASUREITEMSTRUCT) lParam);
            case WM_MENUSELECT:     return OnMenuSelect((UINT) LOWORD(wParam), (UINT) HIWORD(wParam), (HMENU) lParam);
            case WM_MOUSEWHEEL:     return OnMouseWheel((int) LOWORD(wParam), (short) HIWORD(wParam), MAKEPOINTS(lParam));
            case WM_NOTIFY:         return OnNotify((int) wParam, (LPNMHDR) lParam);
            case WM_SETTINGCHANGE:  return OnSettingChange((UINT) wParam, (LPCTSTR) lParam);
            case WM_SETCURSOR:      return OnSetCursor((HWND) wParam, (UINT) LOWORD(lParam), (UINT) HIWORD(lParam));
            case WM_SIZE:           return OnSize((UINT) wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            case WM_SIZING:         return OnSizing((int) wParam, (LPRECT) lParam);
            case WM_TIMER:          return OnTimer((UINT) wParam, (TIMERPROC*) lParam);
            case WM_CLOSE:          return OnClose();
            case WM_DESTROY:        return OnDestroy();
            case WM_NCMOUSEMOVE:    return OnNCMouseMove((UINT) wParam, MAKEPOINTS(lParam));
            case WM_NCDESTROY:      return OnNCDestroy();
            default:                return UnknownMessage(Message, wParam, lParam);
            }
        }
        catch (const WinError& e)
        {
            TCHAR    Text[1024];
            GetWindowText(Text);
            MessageBox(*this, e.GetString().c_str(), Text, MB_OK | MB_ICONSTOP);
            return 0;
        }
        catch (...)
        {
            TCHAR    Text[1024];
            GetWindowText(Text);
            MessageBox(*this, _T("Unknown Exception. Caught in ") _T(__FUNCTION__), Text, MB_OK | MB_ICONSTOP);
            return 0;
        }
    }

    LRESULT Window::OnCreate(LPCREATESTRUCT /*CreateStruct*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnPaint(PaintDC& /*DC*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnMove(POINTS /*Point*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnMouseMove(UINT /*Keys*/, POINTS /*Point*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnMouseLeave()
    {
        return DoDefault();
    }

    LRESULT Window::OnLeftButtonDown(UINT /*Keys*/, POINTS /*Point*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnLeftButtonUp(UINT /*Keys*/, POINTS /*Point*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnLeftButtonDblClk(UINT /*Keys*/, POINTS /*Point*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnRightButtonDown(UINT /*Keys*/, POINTS /*Point*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnRightButtonUp(UINT /*Keys*/, POINTS /*Point*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnRightButtonDblClk(UINT /*Keys*/, POINTS /*Point*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnMiddleButtonDown(UINT /*Keys*/, POINTS /*Point*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnMiddleButtonUp(UINT /*Keys*/, POINTS /*Point*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnMiddleButtonDblClk(UINT /*Keys*/, POINTS /*Point*/)
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

    LRESULT Window::OnContextMenu(HWND /*hWnd*/, POINTS /*Point*/)
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

    LRESULT Window::OnEraseBackground(DevContextRef /*DC*/)
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

    LRESULT Window::OnKeyDown(int /*VirtKey*/, KeyInfoT /*KeyInfo*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnKeyUp(int /*VirtKey*/, KeyInfoT /*KeyInfo*/)
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

    LRESULT Window::OnMouseWheel(int /*Flags*/, short /*Distance*/, POINTS /*Point*/)
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

    LRESULT Window::OnSysKeyDown(int /*VirtKey*/, KeyInfoT /*KeyInfo*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnSysKeyUp(int /*VirtKey*/, KeyInfoT /*KeyInfo*/)
    {
        return DoDefault();
    }

    LRESULT Window::OnSysCommand(WORD /*ID*/, POINTS /*Point*/)
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
        MarkForDelete();
        return DoDefault();
    }

    LRESULT Window::OnNCMouseMove(UINT /*HitTest*/, POINTS /*Point*/)
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
}
