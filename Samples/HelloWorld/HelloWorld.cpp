#include <Rad\GUI\Window.h>
#include <Rad\GUI\DevContext.h>
#include <Rad\GUI\RegClass.h>
#include <Rad\GUI\MessageLoop.h>

using namespace rad;

class WindowHelloWorld : public Window
{
public:
    virtual LRESULT OnPaint(CPaintDC &DC)
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
    WindowHelloWorld* hw = WindowCreate<WindowHelloWorld>(hInstance, _T("Hello World")).Create();
    hw->ShowWindow(nCmdShow);

    return (int) DoMessageLoop();
}
