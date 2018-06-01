#ifndef __WindowMAP_H__
#define __WindowMAP_H__

#include "WindowProxy.H"

#include <map>
#include <vector>

namespace rad
{
    class WindowMap : public WindowProxy
    {
    protected:
        WindowMap()
        {
        }
        WindowMap(WindowMap&) = delete;

    protected:
        void PushWndProcDepth()
        {
            ++m_WndProcDepth;
        }

        void PopWndProcDepth()
        {
            --m_WndProcDepth;

            if (m_WndProcDepth <= 0)
                TestDelete();
        }

        void TestDelete()
        {
            if (m_MarkForDelete)
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
