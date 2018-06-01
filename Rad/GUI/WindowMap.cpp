#include "WindowMap.H"

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
}
