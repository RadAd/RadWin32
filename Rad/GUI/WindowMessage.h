#pragma once

#include <Windows.h>
#include <vector>

namespace rad
{
    class WindowChain;
    class Window;

    struct Msg
    {
        WNDPROC DefWndProc;
        UINT Message;
        WPARAM wParam;
        LPARAM lParam;
    };

    class WindowMessage
    {
    public:
        void Add(WindowChain* Chain)
        {
            m_WindowChain.push_back(Chain);
        }

        LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, WNDPROC DefWndProc);

    protected:
        virtual LRESULT OnMessage(UINT Message, WPARAM wParam, LPARAM lParam) = 0;

    protected:
        LRESULT DoChain(Window* Window);

    private:
        Msg m_LastMessage;
        std::vector<WindowChain*> m_WindowChain;
    };
};
