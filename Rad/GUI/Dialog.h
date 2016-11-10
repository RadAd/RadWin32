#ifndef __DLGHANDLER_H__
#define __DLGHANDLER_H__

#include "WindowMap.H"

namespace rad
{
    class Dialog : public WindowMap
    {
    public:
        void CreateDlg(HINSTANCE hInstance, int TemplateID, HWND hParent);

        INT_PTR DoModal(HINSTANCE hInstance, LPCTSTR Template, HWND hParent);
        INT_PTR DoModal(HINSTANCE hInstance, int TemplateID, HWND hParent);

    protected:
        void EndDialog(int Result)
        {
            assert(IsWindow());
            assert(m_IsModal);
            if (::EndDialog(GetHWND(), Result) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

    protected:
        virtual BOOL OnMessage(UINT Message, WPARAM wParam, LPARAM lParam);
        virtual BOOL OnInitDialog(HWND FocusControl);
        virtual BOOL OnCommand(WORD NotifyCode, WORD ID, HWND hWnd);
        virtual BOOL OnDestroy();
        virtual BOOL UnknownMessage(UINT Message, WPARAM wParam, LPARAM lParam);

        virtual BOOL OnOk();
        virtual BOOL OnCancel();

    private:
        static INT_PTR CALLBACK DlgHandlerWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        bool    m_IsModal = false;
    };
}

#endif //__DLGHANDLER_H__
