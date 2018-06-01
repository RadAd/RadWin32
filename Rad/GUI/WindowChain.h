#pragma once

#include <Windows.h>
#include <utility>

namespace rad
{
    class Window;

    class WindowChain
    {
    public:
        class Handle
        {
        public:
            virtual LRESULT DoNext(Window* Window) = 0;
        };

        LRESULT OnMessage(Window* Window, UINT Message, WPARAM wParam, LPARAM lParam, Handle* handle)
        {
            std::swap(m_handle, handle);
            LRESULT ret = OnMessage(Window, Message, wParam, lParam);
            std::swap(m_handle, handle); // TODO Make exception safe
            return ret;
        }

    protected:
        virtual LRESULT OnMessage(Window* Window, UINT /*Message*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
        {
            return DoDefault(Window);
        }

    protected:
        LRESULT DoDefault(Window* Window)
        {
            return m_handle->DoNext(Window);
        }

    private:
        Handle* m_handle = nullptr;
    };
}
