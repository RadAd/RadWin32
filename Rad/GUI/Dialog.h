#ifndef __DLGHANDLER_H__
#define __DLGHANDLER_H__

#pragma warning(disable : 4786)

#include "WindowProxy.H"
#include "WindowListener.H"
#include <map>
#include <vector>

namespace rad
{
    class Dialog : public WindowProxy
    {
    public:
        Dialog()
        {
        }

        Dialog(HINSTANCE hInstance, int TemplateID, HWND hParent);

        INT_PTR DoModal(HINSTANCE hInstance, LPCTSTR Template, HWND hParent);
        INT_PTR DoModal(HINSTANCE hInstance, int TemplateID, HWND hParent);

    public:
        void EndDialog(int Result)
        {
            assert(IsWindow());
            assert(m_IsModal);
            if (::EndDialog(GetHWND(), Result) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

    public:
        void AddWindowListener(WindowListener* WndListener)
        {
            m_WindowListeners.push_back(WndListener);
        }

        void RemoveWindowListener(WindowListener* WndListener);

    private:
        typedef	std::vector<WindowListener*>	WindowListenersContT;
        WindowListenersContT	m_WindowListeners;

    protected:
        void SetLastMessage(UINT Message, WPARAM wParam, LPARAM lParam)
        {
            m_LastMessage = Message;
            m_LastwParam = wParam;
            m_LastlParam = lParam;
        }

    public:
        virtual BOOL OnMessage(UINT Message, WPARAM wParam, LPARAM lParam);
        virtual BOOL OnInitDialog(HWND FocusControl);
        virtual BOOL OnCommand(WORD NotifyCode, WORD ID, HWND hWnd);
        virtual BOOL OnDestroy();
        virtual BOOL UnknownMessage(UINT Message, WPARAM wParam, LPARAM lParam);

        virtual BOOL OnOk();
        virtual BOOL OnCancel();

    private:
        bool    m_IsModal = false;
        UINT    m_LastMessage = 0;
        WPARAM  m_LastwParam = 0;
        LPARAM  m_LastlParam = 0;

    public:	// static
        static Dialog* FromhWnd(HWND hWnd);
        void AttachMap(HWND hWnd);
        void DetachMap();

    private:
        typedef	std::map<HWND, Dialog*> hWndMapT;
        static hWndMapT     s_hWndMap;
        static int          s_ExitCode;
    };
}

#endif //__DLGHANDLER_H__
