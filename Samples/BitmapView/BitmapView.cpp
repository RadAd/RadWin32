#include <Rad\GUI\Window.h>
#include <Rad\GUI\DevContext.h>
#include <Rad\GUI\GdiObject.h>
#include <Rad\GUI\RegClass.h>
#include <Rad\GUI\MessageLoop.h>
#include <Rad\Rect.h>

#include "resource.h"

using namespace rad;

void DrawBitmap(DevContext& DC, const Bitmap& bmp, POINT p, DWORD RasterOp = SRCCOPY)
{
    SIZE s = bmp.GetSize();
    CMemDC mdc(DC);
    TempSelectObject selectbmp(mdc, bmp);
    DC.BitBlt(p.x, p.y, s.cx, s.cy, mdc, 0, 0, RasterOp);
}

void DrawBitmapCentered(DevContext& DC, const Bitmap& bmp, POINT p, DWORD RasterOp = SRCCOPY)
{
    SIZE s = bmp.GetSize();
    CMemDC mdc(DC);
    TempSelectObject selectbmp(mdc, bmp);
    DC.BitBlt(p.x - s.cx/2, p.y - s.cy/2, s.cx, s.cy, mdc, 0, 0, RasterOp);
}

class WindowBitmapView : public Window
{
public:
    WindowBitmapView(HINSTANCE hInstance, LPCTSTR WindowName)
        : Window(hInstance, WindowName)
    {
        m_logo.Load(hInstance, IDB_WINLOGO);
    }

    virtual LRESULT OnPaint(PaintDC& DC)
    {
        RECT r;
        GetClientRect(&r);

        DrawBitmapCentered(DC, m_logo, GetCenter(r));

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
