#ifndef __WindowPROXY_H__
#define __WindowPROXY_H__

#include <Windows.h>
#include <assert.h>

#include "..\WinError.h"

namespace rad
{
    class WindowProxy
    {
    public:
        static const HWND Invalid;

        WindowProxy(HWND hWnd = Invalid)
            : m_hWnd(Invalid)
        {
            if (hWnd != Invalid)
                Attach(hWnd);
        }

        virtual ~WindowProxy()
        {
            Detach();
        }

        WindowProxy(const WindowProxy& Other)
            : m_hWnd(Other.m_hWnd)
        {
        }

        WindowProxy& operator=(const WindowProxy& Other)
        {
            m_hWnd = Other.m_hWnd;
            return *this;
        }

    public:
        void Attach(HWND hWnd)
        {
            assert(hWnd != Invalid);
            assert(!IsAttached());
            Detach();
            m_hWnd = hWnd;
            assert(IsWindow());
        }

        void Detach()
        {
            m_hWnd = Invalid;
        }

    public:
        bool IsWindow() const
        {
            return ::IsWindow(m_hWnd) != 0;
        }

        bool IsAttached() const
        {
            return (m_hWnd != Invalid);
        }

        void ClientToScreen(LPPOINT Point)
        {
            assert(IsWindow());
            if (::ClientToScreen(m_hWnd, Point) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void ScreenToClient(LPPOINT Point)
        {
            assert(IsWindow());
            if (::ScreenToClient(m_hWnd, Point) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        HWND GetHWND() const
        {
            return m_hWnd;
        }

        LRESULT SendMessage(UINT Message, WPARAM wParam = 0, LPARAM lParam = 0) const
        {
            assert(IsWindow());
            return ::SendMessage(m_hWnd, Message, wParam, lParam);
        }

        LRESULT PostMessage(UINT Message, WPARAM wParam = 0, LPARAM lParam = 0) const
        {
            assert(IsWindow());
            return ::PostMessage(m_hWnd, Message, wParam, lParam);
        }

        void InvalidateRect(const LPRECT pRect = nullptr, bool Erase = true)
        {
            assert(IsWindow());
            if (::InvalidateRect(m_hWnd, pRect, Erase) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        bool EnableWindow(bool Enable)
        {
            assert(IsWindow());
            return ::EnableWindow(m_hWnd, Enable) != 0;
        }

        void DragAcceptFiles(bool Accept = true)
        {
            assert(IsWindow());
            ::DragAcceptFiles(m_hWnd, Accept);
        }

        void GetClientRect(LPRECT pRect) const
        {
            assert(IsWindow());
            if (::GetClientRect(m_hWnd, pRect) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        int GetClassName(LPTSTR String, int MaxLength) const
        {
            assert(IsWindow());
            return ::GetClassName(m_hWnd, String, MaxLength);
        }

        template<std::size_t N>
        int GetClassName(TCHAR(&String)[N]) const
        {
            assert(IsWindow());
            return ::GetClassName(m_hWnd, String, N);
        }

        ULONG_PTR GetClassLongPtr(int Index) const
        {
            assert(IsWindow());
            return ::GetClassLongPtr(m_hWnd, Index);
        }

        HFONT GetFont() const
        {
            return (HFONT) SendMessage(WM_GETFONT);
        }

        HICON GetIcon(int Type) const
        {
            return (HICON) SendMessage(WM_GETICON, Type);
        }

        HMENU GetMenu() const
        {
            assert(IsWindow());
            return ::GetMenu(m_hWnd);
        }

        HWND GetParent() const
        {
            assert(IsWindow());
            return ::GetParent(m_hWnd);
        }

        HWND GetAncestor(UINT gaFlags) const
        {
            assert(IsWindow());
            return ::GetAncestor(m_hWnd, gaFlags);
        }

        DWORD GetProcessId() const
        {
            assert(IsWindow());
            DWORD    ProcessId = 0;
            ::GetWindowThreadProcessId(m_hWnd, &ProcessId);
            return ProcessId;
        }

        DWORD GetThreadId() const
        {
            assert(IsWindow());
            DWORD    ProcessId = 0;
            return ::GetWindowThreadProcessId(m_hWnd, &ProcessId);
        }

        HANDLE GetProp(LPCTSTR String) const
        {
            assert(IsWindow());
            return ::GetProp(m_hWnd, String);
        }

        void GetScrollInfo(int fnBar, LPSCROLLINFO lpsi) const
        {
            assert(IsWindow());
            if (::GetScrollInfo(m_hWnd, fnBar, lpsi) == 0)
            {
                WinError Error(_T(__FUNCTION__));
                if ((Error.GetError() == ERROR_NO_SCROLLBARS) || (Error.GetError() == NO_ERROR))
                {
                    lpsi->nMin = 0;
                    lpsi->nMax = 0;
                    lpsi->nPos = 0;
                    lpsi->nPage = 0;
                    lpsi->nPage = 0;
                }
                else
                    ThrowWinError(Error);
            }
        }

        void GetScrollRange(int fnBar, LPINT Min, LPINT Max) const
        {
            assert(IsWindow());
            if (::GetScrollRange(m_hWnd, fnBar, Min, Max) == 0)
            {
                WinError Error(_T(__FUNCTION__));
                if ((Error.GetError() == ERROR_NO_SCROLLBARS) || (Error.GetError() == NO_ERROR))
                {
                    Min = 0;
                    Max = 0;
                }
                else
                    ThrowWinError(Error);
            }
        }

        int GetScrollPos(int fnBar) const
        {
            assert(IsWindow());
            SetLastError(0);
            int Pos = ::GetScrollPos(m_hWnd, fnBar);
            if (Pos == 0)
            {
                WinError Error(_T(__FUNCTION__));
                if ((Error.GetError() != NO_ERROR) && (Error.GetError() != ERROR_NO_SCROLLBARS))
                    ThrowWinError(Error);
            }
            return Pos;
        }

        HMENU GetSystemMenu(BOOL bRevert) const
        {
            return ::GetSystemMenu(m_hWnd, bRevert);
        }

        WindowProxy GetWindow(int Index) const
        {
            assert(IsWindow());
            return ::GetWindow(m_hWnd, Index);
        }

        LONG_PTR GetWindowLongPtr(int Index) const
        {
            assert(IsWindow());
            return ::GetWindowLongPtr(m_hWnd, Index);
        }

        void GetWindowPlacement(LPWINDOWPLACEMENT WindowPlacement) const
        {
            assert(IsWindow());
            if (::GetWindowPlacement(m_hWnd, WindowPlacement) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void GetWindowRect(LPRECT pRect) const
        {
            assert(IsWindow());
            if (::GetWindowRect(m_hWnd, pRect) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        int GetWindowText(LPTSTR String, int MaxLength) const
        {
            assert(IsWindow());
            return ::GetWindowText(m_hWnd, String, MaxLength);
        }

        template<std::size_t N>
        int GetWindowText(TCHAR(&String)[N]) const
        {
            assert(IsWindow());
            return ::GetWindowText(m_hWnd, String, N);
        }

        int GetWindowTextLength() const
        {
            assert(IsWindow());
            return ::GetWindowTextLength(m_hWnd);
        }

        bool IsWindowEnabled() const
        {
            assert(IsWindow());
            return ::IsWindowEnabled(m_hWnd) != 0;
        }

        bool IsWindowVisible() const
        {
            assert(IsWindow());
            return ::IsWindowVisible(m_hWnd) != 0;
        }

        void RegisterHotKey(UINT Modifier, UINT VKey, int Id = 0)
        {
            assert(IsWindow());
            if (::RegisterHotKey(m_hWnd, Id, Modifier, VKey) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void ReleaseCapture()
        {
            assert(IsWindow());
            if (::ReleaseCapture() == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void RemoveProp(LPCTSTR String) const
        {
            assert(IsWindow());
            if (::RemoveProp(m_hWnd, String) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void MoveWindow(int X, int Y, int Width, int Height, BOOL Repaint)
        {
            assert(IsWindow());
            if (::MoveWindow(m_hWnd, X, Y, Width, Height, Repaint) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void SetFocus() const
        {
            assert(IsWindow());
            if (::SetFocus(m_hWnd) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void SetProp(LPCTSTR String, HANDLE h) const
        {
            assert(IsWindow());
            if (::SetProp(m_hWnd, String, h) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        int SetScrollInfo(int fnBar, LPSCROLLINFO lpsi, bool Redraw)
        {
            assert(IsWindow());
            return ::SetScrollInfo(m_hWnd, fnBar, lpsi, Redraw);
        }

        void SetScrollRange(int fnBar, int Min, int Max, bool Redraw)
        {
            assert(IsWindow());
            if (::SetScrollRange(m_hWnd, fnBar, Min, Max, Redraw) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void SetScrollPos(int fnBar, int Min, int Max, bool Redraw)
        {
            assert(IsWindow());
            if (::SetScrollRange(m_hWnd, fnBar, Min, Max, Redraw) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        UINT SetTimer(UINT IDEvent, UINT Elapse, TIMERPROC TimerFunc = nullptr)
        {
            UINT ID = (UINT) ::SetTimer(m_hWnd, IDEvent, Elapse, TimerFunc);
            if (ID == 0)
                ThrowWinError(_T(__FUNCTION__));
            return ID;
        }

        void SetWindowPlacement(const LPWINDOWPLACEMENT WindowPlacement)
        {
            assert(IsWindow());
            if (::SetWindowPlacement(m_hWnd, WindowPlacement) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void SetWindowPos(
            const WindowProxy& Next,
            int X,
            int Y,
            int cx,
            int cy,
            UINT uFlags)
        {
            assert(IsWindow());
            if (::SetWindowPos(m_hWnd, Next.m_hWnd, X, Y, cx, cy, uFlags) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void SetWindowText(LPCTSTR String)
        {
            assert(IsWindow());
            if (::SetWindowText(m_hWnd, String) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        HWND SetActiveWindow()
        {
            assert(IsWindow());
            return ::SetActiveWindow(m_hWnd);
        }

        HWND SetCapture()
        {
            assert(IsWindow());
            return ::SetCapture(m_hWnd);
        }

        bool SetForegroundWindow()
        {
            assert(IsWindow());
            return ::SetForegroundWindow(m_hWnd) != 0;
        }

        void SetFont(HFONT Font, BOOL bRedraw)
        {
            SendMessage(WM_SETFONT, (WPARAM) Font, MAKELPARAM(bRedraw, 0));
        }

        HICON SetIcon(int Type, HICON Icon)
        {
            return (HICON) SendMessage(WM_SETICON, Type, (LPARAM) Icon);
        }

        long SetWindowLong(int Index, long Value)
        {
            assert(IsWindow());
            SetLastError(0);
            long    LastValue = ::SetWindowLong(m_hWnd, Index, Value);
            if (LastValue == 0)
            {
                WinError Error(_T(__FUNCTION__));
                if (Error.GetError() != NO_ERROR)
                    ThrowWinError(Error);
            }
            return LastValue;
        }

        bool ShowWindow(int CmdShow = SW_SHOW)
        {
            assert(IsWindow());
            return ::ShowWindow(m_hWnd, CmdShow) != 0;
        }

        void ShowScrollBar(int fnBar, bool Show)
        {
            assert(IsWindow());
            if (::ShowScrollBar(m_hWnd, fnBar, Show) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void CloseWindow()
        {
            assert(IsWindow());
            if (::CloseWindow(m_hWnd) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void DestroyWindow()
        {
            assert(IsWindow());
            if (::DestroyWindow(m_hWnd) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        WindowProxy SetClipboardViewer()
        {
            assert(IsWindow());
            SetLastError(NO_ERROR);
            HWND    Next = ::SetClipboardViewer(m_hWnd);
            if (Next == NULL)
            {
                WinError Error(_T(__FUNCTION__));
                if ((Error.GetError() != NO_ERROR) && (Error.GetError() != ERROR_FILE_NOT_FOUND) && (Error.GetError() != ERROR_ALREADY_EXISTS))
                {
                    ThrowWinError(Error);
                }
            }
            return WindowProxy(Next);
        }

        void UpdateWindow()
        {
            assert(IsWindow());
            if (::UpdateWindow(m_hWnd) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void ChangeClipboardChain(WindowProxy& Next)
        {
            assert(IsWindow());
            ::ChangeClipboardChain(m_hWnd, Next.m_hWnd);
            // Not always an error, could just be last in chain
            //if (::ChangeClipboardChain(m_hWnd, Next.m_hWnd) == 0)
            //{
            //    ThrowWinError(_T(__FUNCTION__));
            //}
        }

    public: //Dialog functions
        WindowProxy GetDlgItem(int DlgItemID)
        {
            assert(IsWindow());
            HWND    hWnd = ::GetDlgItem(m_hWnd, DlgItemID);
            if (hWnd == NULL)
                ThrowWinError(_T(__FUNCTION__));
            return WindowProxy(hWnd);
        }

        UINT GetDlgItemTextA(int DlgItemID, LPSTR String, int MaxLength)
        {
            assert(IsWindow());
            UINT    Copied = ::GetDlgItemTextA(m_hWnd, DlgItemID, String, MaxLength);
            if (Copied == 0)
            {
                WinError Error(_T(__FUNCTION__));
                if ((Error.GetError() != NO_ERROR))
                    ThrowWinError(Error);
            }
            return Copied;
        }

        UINT GetDlgItemTextW(int DlgItemID, LPWSTR String, int MaxLength)
        {
            assert(IsWindow());
            UINT    Copied = ::GetDlgItemTextW(m_hWnd, DlgItemID, String, MaxLength);
            if (Copied == 0)
            {
                WinError Error(_T(__FUNCTION__));
                if ((Error.GetError() != NO_ERROR))
                    ThrowWinError(Error);
            }
            return Copied;
        }

        void SetDlgItemTextA(int DlgItemID, LPCSTR String)
        {
            assert(IsWindow());
            if (::SetDlgItemTextA(m_hWnd, DlgItemID, String) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void SetDlgItemTextW(int DlgItemID, LPCWSTR String)
        {
            assert(IsWindow());
            if (::SetDlgItemTextW(m_hWnd, DlgItemID, String) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        UINT GetDlgItemInt(int DlgItemID, bool Signed)
        {
            assert(IsWindow());
            BOOL    Success;
            UINT    Value = ::GetDlgItemInt(m_hWnd, DlgItemID, &Success, Signed);
            if (Success == FALSE)
            {
                WinError Error(_T(__FUNCTION__));
                if ((Error.GetError() != NO_ERROR))
                    ThrowWinError(Error);
            }
            return Value;
        }

        void SetDlgItemInt(int DlgItemID, UINT Value, bool Signed)
        {
            assert(IsWindow());
            if (::SetDlgItemInt(m_hWnd, DlgItemID, Value, Signed) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

    private:
        HWND    m_hWnd;
    };

    inline int MessageBox(WindowProxy& Window, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
    {
        return MessageBox(Window.GetHWND(), lpText, lpCaption, uType);
    }
}

#endif //__WindowPROXY_H__
