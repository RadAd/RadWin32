#include "Dialog.H"

namespace rad
{
    Dialog::hWndMapT Dialog::s_hWndMap;
    int Dialog::s_ExitCode = 0;

    INT_PTR CALLBACK DlgHandlerWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        static int depth = 0;
        BOOL RetVal = FALSE;

        ++depth;
        try
        {
            Dialog* DlgHandler = Dialog::FromhWnd(hWnd);

            if (uMsg == WM_INITDIALOG)
            {
                assert(DlgHandler == NULL);
                DlgHandler = (Dialog*) lParam;
                DlgHandler->AttachMap(hWnd);
            }

            if (DlgHandler != NULL)
            {
                RetVal = DlgHandler->OnMessage(uMsg, wParam, lParam);

                if (!IsWindow(hWnd) && depth <= 1)
                {
                    DlgHandler->DetachMap();
                    //delete DlgHandler;
                }
            }
            else
            {
#ifdef	_DEBUG
                switch (uMsg)
                {
                case WM_GETMINMAXINFO:
                case WM_NCCREATE:
                case WM_NCCALCSIZE:
                case WM_NCDESTROY:
                    RetVal = FALSE;
                    break;

                default:
                    RetVal = FALSE;
                    break;
                }
#else
                RetVal = FALSE;
#endif
            }
        }
        catch (const WinError& e)
        {
            TCHAR	Text[1024];
            GetWindowText(hWnd, Text, std::extent<decltype(Text)>::value);
            MessageBox(hWnd, e.GetString().c_str(), Text, MB_OK | MB_ICONSTOP);
        }
        catch (...)
        {
            TCHAR	Text[1024];
            GetWindowText(hWnd, Text, std::extent<decltype(Text)>::value);
            MessageBox(hWnd, _T("Unknown Exception. Caught in ") _T(__FUNCTION__), Text, MB_OK | MB_ICONSTOP);
        }
        --depth;

        return RetVal;
    }

    Dialog::Dialog(HINSTANCE hInstance, int Resource, HWND hParent)
    {
        CreateDialogParam(hInstance, MAKEINTRESOURCE(Resource), hParent, DlgHandlerWindowProc, (LPARAM) this);
    }

    INT_PTR Dialog::DoModal(HINSTANCE hInstance, LPCTSTR Template, HWND hParent)
    {
        m_IsModal = true;
        INT_PTR  ret = DialogBoxParam(hInstance, Template, hParent, DlgHandlerWindowProc, (LPARAM) this);
        DetachMap();
        m_IsModal = false;
        return ret;
    }

    INT_PTR Dialog::DoModal(HINSTANCE hInstance, int TemplateID, HWND hParent)
    {
        return DoModal(hInstance, MAKEINTRESOURCE(TemplateID), hParent);
    }

    Dialog* Dialog::FromhWnd(HWND hWnd)
    {
        hWndMapT::iterator	it = s_hWndMap.find(hWnd);

        if (it == s_hWndMap.end())
            return NULL;
        else
            return it->second;
    }

    void Dialog::AttachMap(HWND hWnd)
    {
        Attach(hWnd);
        s_hWndMap.insert(hWndMapT::value_type(GetHWND(), this));
    }

    void Dialog::DetachMap()
    {
        s_hWndMap.erase(GetHWND());
        if (m_IsModal)
            Detach();
        else
        {
            if (s_hWndMap.size() == 0)		// Last window has been destroyed
                PostQuitMessage(s_ExitCode);
            delete this;
        }
    }

    BOOL Dialog::OnMessage(UINT Message, WPARAM wParam, LPARAM lParam)
    {
        BOOL		RetVal = 0;

        try
        {
            SetLastMessage(Message, wParam, lParam);

            for (WindowListener* pListener : m_WindowListeners)
                pListener->OnPreMessage(this, Message, wParam, lParam);

            switch (Message)
            {
            case WM_INITDIALOG:
                RetVal = OnInitDialog((HWND) wParam);
                break;

            case WM_COMMAND:
                RetVal = OnCommand(HIWORD(wParam), LOWORD(wParam), (HWND) lParam);
                break;

            case WM_DESTROY:
                RetVal = OnDestroy();
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
            TCHAR	Text[1024];
            GetWindowText(Text);
            MessageBox(*this, e.GetString().c_str(), Text, MB_OK | MB_ICONSTOP);
        }
        catch (...)
        {
            TCHAR	Text[1024];
            GetWindowText(Text);
            MessageBox(*this, _T("Unknown Exception. Caught in ") _T(__FUNCTION__), Text, MB_OK | MB_ICONSTOP);
        }

        return RetVal;
    }

    BOOL Dialog::OnInitDialog(HWND /*FocusControl*/)
    {
        return TRUE;
    }

    BOOL Dialog::OnCommand(WORD /*NotifyCode*/, WORD ID, HWND /*hWnd*/)
    {
        BOOL	RetVal;
        switch (ID)
        {
        case IDOK:
            RetVal = OnOk();
            break;

        case IDCANCEL:
            RetVal = OnCancel();
            break;

        default:
            RetVal = FALSE;
        }
        return RetVal;
    }

    BOOL Dialog::OnDestroy()
    {
        return TRUE;
    }

    BOOL Dialog::UnknownMessage(UINT /*Message*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
    {
        return FALSE;
    }

    BOOL Dialog::OnOk()
    {
        if (m_IsModal)
            EndDialog(true);
        else
            DestroyWindow();
        return TRUE;
    }

    BOOL Dialog::OnCancel()
    {
        if (m_IsModal)
            EndDialog(false);
        else
            DestroyWindow();
        return TRUE;
    }

    void Dialog::RemoveWindowListener(WindowListener* pWindowListener)
    {
        WindowListenersContT::iterator it = std::find(m_WindowListeners.begin(), m_WindowListeners.end(), pWindowListener);
        if (it != m_WindowListeners.end())
            m_WindowListeners.erase(it);
    }
}
