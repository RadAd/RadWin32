#include <Rad\GUI\Window.h>
#include <Rad\GUI\DevContext.h>
#include <Rad\GUI\GdiObject.h>
#include <Rad\GUI\RegClass.h>
#include <Rad\GUI\MessageLoop.h>

#include "resource.h"

using namespace rad;

class WindowBitmapView : public Window
{
public:
    WindowBitmapView(HINSTANCE hInstance, LPCTSTR WindowName)
        : Window(hInstance, WindowName)
    {
        m_logo.Load(hInstance, IDB_WINLOGO);
    }

    virtual LRESULT OnPaint(CPaintDC &DC)
    {
        RECT r;
        GetClientRect(&r);
        BITMAP bm;
        m_logo.GetObject(&bm);

        CMemDC logoDC(DC);
        TempSelectObject selectLogo(logoDC, m_logo);
        DC.BitBlt((r.left + r.right - bm.bmWidth)/2, (r.top + r.bottom - bm.bmHeight)/2, bm.bmWidth, bm.bmHeight, logoDC, 0, 0, SRCCOPY);

        return Window::OnPaint(DC);
    }
private:
    Bitmap m_logo;
};

int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_ HINSTANCE /*hPrevInstance*/,
    _In_ LPSTR     /*lpCmdLine*/,
    _In_ int       nCmdShow
)
{
    WindowBitmapView* bv = new WindowBitmapView(hInstance, _T("Bitmap View"));
    bv->ShowWindow(nCmdShow);

    return (int) DoMessageLoop();
}
