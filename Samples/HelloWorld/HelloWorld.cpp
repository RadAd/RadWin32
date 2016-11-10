#include <Rad\GUI\Window.h>
#include <Rad\GUI\DevContext.h>
#include <Rad\GUI\MessageLoop.h>

using namespace rad;

class WindowHelloWorld : public Window
{
public:
    static WindowHelloWorld* Create(HINSTANCE hInstance)
    {
        WindowHelloWorld* w = new WindowHelloWorld();
        w->CreateWnd(hInstance, _T("Hello World"));
        return w;
    }

protected:
    virtual LRESULT OnPaint(PaintDC& DC)
    {
        RECT r;
        GetClientRect(&r);
        DC.DrawText(_T("Hello, World!"), -1, &r, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

        return Window::OnPaint(DC);
    }
};

int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_ HINSTANCE /*hPrevInstance*/,
    _In_ LPSTR     /*lpCmdLine*/,
    _In_ int       nCmdShow
)
{
    WindowHelloWorld* w = WindowHelloWorld::Create(hInstance);
    w->ShowWindow(nCmdShow);

    return (int) DoMessageLoop();
}
