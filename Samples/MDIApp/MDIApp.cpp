#include <Rad\GUI\Window.h>
#include <Rad\GUI\MDIFrame.h>
#include <Rad\GUI\WindowCreate.h>
#include <Rad\GUI\RegClass.h>
#include <Rad\GUI\MessageLoop.h>

#include <WinUser.h>

using namespace rad;

int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_ HINSTANCE /*hPrevInstance*/,
    _In_ LPSTR     /*lpCmdLine*/,
    _In_ int       nCmdShow
)
{
    MDIFrame* hw = new MDIFrame();
    hw->CreateWnd(hInstance, _T("MDI App"));
    hw->ShowWindow(nCmdShow);

    (new Window())->CreateMDIChildWnd(_T("MDI Child A"), hw);

    //CreateWnd(new Window(), _T("Window 1"));
    //CreateMDIChild(hw, new Window(), _T("MDI Child 1"));

    hw->CreateChild(new Window(), _T("MDI Child 1"));
    hw->CreateChild(new Window(), _T("MDI Child 2"));
    hw->CreateChild(new Window(), _T("MDI Child 3"));

    return (int) DoMessageLoop(hw->GetHWND(), hw->GetMDIClient().GetHWND(), NULL);
}
