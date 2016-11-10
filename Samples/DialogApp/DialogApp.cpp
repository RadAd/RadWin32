#include <Rad\GUI\Dialog.h>
#include <Rad\GUI\MessageLoop.h>

#include "resource.h"

using namespace rad;

int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_ HINSTANCE /*hPrevInstance*/,
    _In_ LPSTR     /*lpCmdLine*/,
    _In_ int       nCmdShow
)
{
#if 1   // Modal dialog
    nCmdShow;   // Unreferenced
    Dialog dlg;
    return (int) dlg.DoModal(hInstance, IDD_DIALOG1, NULL);
#else   // Modeless dialog
    Dialog* dlg = new Dialog();
    dlg->CreateDlg(hInstance, IDD_DIALOG1, NULL);
    dlg->ShowWindow(nCmdShow);

    return (int) DoMessageLoop();
#endif
}
