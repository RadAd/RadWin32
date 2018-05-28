#ifndef __WindowREGISTER_H__
#define __WindowREGISTER_H__

#include <Windows.h>

namespace rad
{
    class RegClass : public WNDCLASSEX
    {
    public:
        RegClass(HINSTANCE _hInstance, LPCTSTR ClassName, WNDPROC WndProc);
        ATOM Register() const;

        void SetIcon(WORD res) { hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(res)); }
        void SetIcon(LPCTSTR res) { hIcon = LoadIcon(hInstance, res); }
        void SetCursor(WORD res) { hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(res)); }
        void SetCursor(LPCTSTR res) { hCursor = LoadCursor(hInstance, res); }
        void SetBackgroundColorType(int c) { hbrBackground = (HBRUSH) (INT_PTR) (c + 1); }
    };
}

#endif
