#ifndef __WindowLISTENER_H__
#define __WindowLISTENER_H__

namespace rad
{
    class WindowProxy;

    class WindowListener
    {
    public:
        virtual LRESULT OnPreMessage(WindowProxy* /*Window*/, UINT /*Message*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
        {
            return 0;
        }
        virtual LRESULT OnPostMessage(WindowProxy* /*Window*/, UINT /*Message*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
        {
            return 0;
        }
    };
}

#endif
