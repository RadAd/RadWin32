#include "MessageLoop.H"

namespace rad
{
    WPARAM DoMessageLoop()
    {
        MSG    Message;

        // Exits when it recieves a WM_QUIT message
        while (GetMessage(&Message, NULL, 0, 0))
        {
            TranslateMessage(&Message);
            DispatchMessage(&Message);
        }

        return Message.wParam;
    }

    WPARAM DoMessageLoop(HWND hWnd, HACCEL hAccel)
    {
        MSG    Message;

        // Exits when it recieves a WM_QUIT message
        while (GetMessage(&Message, NULL, 0, 0))
        {
            if (!TranslateAccelerator(hWnd, hAccel, &Message))
            {
                TranslateMessage(&Message);
                DispatchMessage(&Message);
            }
        }

        return Message.wParam;
    }

    WPARAM DoMessageLoop(HACCEL hAccel)
    {
        MSG    Message;

        // Exits when it recieves a WM_QUIT message
        while (GetMessage(&Message, NULL, 0, 0))
        {
            if (!TranslateAccelerator(Message.hwnd, hAccel, &Message))
            {
                TranslateMessage(&Message);
                DispatchMessage(&Message);
            }
        }

        return Message.wParam;
    }

    WPARAM DoMessageLoop(HWND hWndFrame, HWND hWndClient, HACCEL hAccel)
    {
        MSG    Message;

        // Exits when it recieves a WM_QUIT message
        while (GetMessage(&Message, NULL, 0, 0))
        {
            if (!TranslateMDISysAccel(hWndClient, &Message) &&
                !TranslateAccelerator(hWndFrame, hAccel, &Message))
            {
                TranslateMessage(&Message);
                DispatchMessage(&Message);
            }
        }

        return Message.wParam;
    }
}
