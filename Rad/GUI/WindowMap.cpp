#include "WindowMap.H"

#include "WindowListener.h"
//#include "..\..\Logging.H"

namespace rad
{
    const HWND WindowProxy::Invalid = NULL;

    WindowMap::HWNDMapT WindowMap::s_HWNDMap;
    int WindowMap::s_ExitCode = 0;

    WindowMap* WindowMap::FromHWND(HWND hWnd)
    {
        HWNDMapT::const_iterator    it = s_HWNDMap.find(hWnd);

        if (it == s_HWNDMap.end())
            return nullptr;
        else
            return it->second;
    }

    void WindowMap::AttachMap(HWND hWnd)
    {
        Attach(hWnd);
        s_HWNDMap.insert(HWNDMapT::value_type(GetHWND(), this));
    }

    void WindowMap::DetachMap()
    {
        s_HWNDMap.erase(GetHWND());
        if (s_HWNDMap.size() == 0)        // Last window has been destroyed
            PostQuitMessage(s_ExitCode);
    }

    void WindowMap::RemoveWindowListener(WindowListener* pWindowListener)
    {
        WindowListenersContT::iterator it = std::find(m_WindowListeners.begin(), m_WindowListeners.end(), pWindowListener);
        if (it != m_WindowListeners.end())
            m_WindowListeners.erase(it);
    }

    void WindowMap::DoPreMessage(UINT Message, WPARAM wParam, LPARAM lParam)
    {
        for (WindowListener* pListener : m_WindowListeners)
            pListener->OnPreMessage(this, Message, wParam, lParam);
    }

    void WindowMap::DoPostMessage(UINT Message, WPARAM wParam, LPARAM lParam)
    {
        for (WindowListener* pListener : m_WindowListeners)
            pListener->OnPostMessage(this, Message, wParam, lParam);
    }
}
