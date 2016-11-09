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
    };
}

#endif
