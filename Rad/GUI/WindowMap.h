#ifndef __WindowMAP_H__
#define __WindowMAP_H__

#include "WindowProxy.H"

#include <map>
#include <vector>

namespace rad
{
    class DevContextRef;
    class WindowListener;

    class WindowMap : public WindowProxy
    {
    protected:
        WindowMap()
        {
        }
        WindowMap(WindowMap&) = delete;

    public:
        void AddWindowListener(WindowListener* pWindowListener)
        {
            m_WindowListeners.push_back(pWindowListener);
        }

        void RemoveWindowListener(WindowListener* pWindowListener);

    protected:
        void DoPreMessage(UINT Message, WPARAM wParam, LPARAM lParam);
        void DoPostMessage(UINT Message, WPARAM wParam, LPARAM lParam);

    private:
        typedef std::vector<WindowListener*> WindowListenersContT;
        WindowListenersContT    m_WindowListeners;

    protected:
        void PushWndProcDepth()
        {
            ++m_WndProcDepth;
        }

        void PopWndProcDepth()
        {
            --m_WndProcDepth;

            if (m_MarkForDelete && m_WndProcDepth <= 0)
            {
                DetachMap();
                delete this;
            }
        }

        void MarkForDelete()
        {
            m_MarkForDelete = true;
        }

    private:
        int m_WndProcDepth = 0;
        int m_MarkForDelete = false;

    protected:    // static
        static WindowMap* FromHWND(HWND hWnd);
        void AttachMap(HWND hWnd);
        void DetachMap();

    private:
        typedef std::map<HWND, WindowMap*> HWNDMapT;
        static HWNDMapT     s_HWNDMap;
        static int          s_ExitCode;
    };
}

#endif //__WindowMAP_H__
