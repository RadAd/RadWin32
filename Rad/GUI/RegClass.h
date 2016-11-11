#ifndef __WindowREGISTER_H__
#define __WindowREGISTER_H__

#include <Windows.h>

namespace rad
{
    class RegClass : public WNDCLASSEX
    {
    public:
        RegClass(HINSTANCE _hInstance, LPCTSTR ClassName);
        ATOM Register();

        static ATOM GetSimple(HINSTANCE _hInstance);
        static ATOM GetMDIFrame(HINSTANCE _hInstance);
        static ATOM GetMDIChild(HINSTANCE _hInstance);

        static RegClass GetMDIFrameClass(HINSTANCE _hInstance);
        static RegClass GetMDIChildClass(HINSTANCE _hInstance);
    };
}

#endif
