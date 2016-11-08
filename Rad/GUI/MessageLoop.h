#ifndef __MESSAGELOOP_H__
#define __MESSAGELOOP_H__

#include <Windows.H>

namespace rad
{
    WPARAM DoMessageLoop();
    WPARAM DoMessageLoop(HWND hWnd, HACCEL hAccel);
    WPARAM DoMessageLoop(HACCEL hAccel);
    WPARAM DoMessageLoop(HWND hWndFrame, HWND hWndClient, HACCEL hAccel);
}

#endif
