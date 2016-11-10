#include <Rad\GUI\Window.h>
#include <Rad\GUI\DevContext.h>
#include <Rad\GUI\GdiObject.h>
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
    static WindowBitmapView* Create(HINSTANCE hInstance)
    {
        WindowBitmapView* w = new WindowBitmapView(hInstance);
        w->CreateWnd(hInstance, _T("Bitmap View"));
        return w;
    }

protected:
    WindowBitmapView(HINSTANCE hInstance)
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
    WindowBitmapView* w = WindowBitmapView::Create(hInstance);
    w->ShowWindow(nCmdShow);

    return (int) DoMessageLoop();
}
