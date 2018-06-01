#include "WindowMessage.h"

#include "WindowChain.h"
#include "Window.h"

namespace
{
    class LocalWindowChain : public rad::WindowChain::Handle
    {
    public:
        LocalWindowChain(const std::vector<rad::WindowChain*>& WindowChain, const rad::Msg& LastMessage)
            : m_WindowChain(WindowChain)
            , m_CurrentChain(m_WindowChain.begin())
            , m_LastMessage(LastMessage)
        {
        }

        virtual LRESULT DoNext(rad::Window* Window) override
        {
            if (m_CurrentChain != m_WindowChain.end())
                //return (*std::exchange(m_CurrentChain, std::next(m_CurrentChain)))->OnMessage(Window, m_LastMessage.Message, m_LastMessage.wParam, m_LastMessage.lParam, this);
                return (*m_CurrentChain++)->OnMessage(Window, m_LastMessage.Message, m_LastMessage.wParam, m_LastMessage.lParam, this);
            else
                return m_LastMessage.DefWndProc(Window->GetHWND(), m_LastMessage.Message, m_LastMessage.wParam, m_LastMessage.lParam);
        }

    private:
        const std::vector<rad::WindowChain*>& m_WindowChain;
        std::vector<rad::WindowChain*>::const_iterator m_CurrentChain;

        const rad::Msg& m_LastMessage;
    };
}

namespace rad
{
    LRESULT WindowMessage::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, WNDPROC DefWndProc)
    {
        Msg msg = { DefWndProc, uMsg, wParam, lParam };
        std::swap(m_LastMessage, msg);
        LRESULT RetVal = OnMessage(uMsg, wParam, lParam);
        std::swap(m_LastMessage, msg); // TODO Make exception safe
        return RetVal;
    }

    LRESULT WindowMessage::DoChain(Window* Window)
    {
        return LocalWindowChain(m_WindowChain, m_LastMessage).DoNext(Window);
    }
}
