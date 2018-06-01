#ifndef __WindowMAP_H__
#define __WindowMAP_H__

#include "WindowProxy.H"

#include <map>
#include <vector>

namespace rad
{
    class WindowDelete;

    class WindowMap
    {
    private:
        WindowMap()
        {
        }

    public:
        static WindowMap* GetInstance();

        WindowDelete* FromHWND(HWND hWnd);
        void AttachMap(WindowDelete* Wnd);
        void DetachMap(WindowDelete* Wnd);

    private:
        typedef std::map<HWND, WindowDelete*> HWNDMapT;
        HWNDMapT     m_HWNDMap;
        int          m_ExitCode;
    };

    class WindowDelete : public WindowProxy
    {
    protected:
        WindowDelete()
        {
        }
        WindowDelete(WindowDelete&) = delete;

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
                WindowMap::GetInstance()->DetachMap(this);
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
    };
}

#endif //__WindowMAP_H__
