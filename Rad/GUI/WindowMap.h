#ifndef __WindowMAP_H__
#define __WindowMAP_H__

#include "WindowProxy.H"

#include <map>
#include <vector>

namespace rad
{
    class DevContext;
    class PaintDC;
    class WindowCreate;
    class WindowListener;

    class WindowMap : public WindowProxy
    {
    protected:
        WindowMap() { }

    protected:
        void SetLastMessage(UINT Message, WPARAM wParam, LPARAM lParam)
        {
            m_LastMessage = Message;
            m_LastwParam = wParam;
            m_LastlParam = lParam;
        }

        UINT GetLastMessage() const { return m_LastMessage; }
        WPARAM GetLastwParam() const { return m_LastwParam; }
        LPARAM GetLastlParam() const { return m_LastlParam; }

    private:
        UINT    m_LastMessage = 0;
        WPARAM  m_LastwParam = 0;
        LPARAM  m_LastlParam = 0;

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
