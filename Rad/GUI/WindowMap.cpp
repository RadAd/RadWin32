#include "WindowMap.H"

//#include "..\..\Logging.H"

namespace rad
{
    const HWND WindowProxy::Invalid = NULL;

    WindowMap* WindowMap::GetInstance()
    {
        static WindowMap wm;
        return &wm;
    }

    WindowDelete* WindowMap::FromHWND(HWND hWnd)
    {
        HWNDMapT::const_iterator    it = m_HWNDMap.find(hWnd);

        if (it == m_HWNDMap.end())
            return nullptr;
        else
            return it->second;
    }

    void WindowMap::AttachMap(WindowDelete* Wnd)
    {
        m_HWNDMap.insert(HWNDMapT::value_type(Wnd->GetHWND(), Wnd));
    }

    void WindowMap::DetachMap(WindowDelete* Wnd)
    {
        m_HWNDMap.erase(Wnd->GetHWND());
        if (m_HWNDMap.size() == 0)        // Last window has been destroyed
            PostQuitMessage(m_ExitCode);
    }
}
