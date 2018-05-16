#include "CommCtrl.h"

#include <memory>
#include <vector>

#define RT_TOOLBAR  MAKEINTRESOURCE(241)	// from "afxres.h"

namespace
{
    struct SToolBarData
    {
        WORD wVersion;
        WORD wWidth;
        WORD wHeight;
        WORD wItemCount;
        //WORD aItems[wItemCount]

        WORD* items()
        {
            return (WORD*) (this + 1);
        }
    };
}

bool rad::ToolBarWnd::Create(const WindowProxy &Parent, DWORD Style, UINT ID, HINSTANCE hInstance, UINT ResID)
{
    HRSRC hRsrc = ::FindResource(hInstance, MAKEINTRESOURCE(ResID), RT_TOOLBAR);
    if (hRsrc == NULL) return false;

    std::unique_ptr<std::remove_pointer<HGLOBAL>::type, BOOL(__stdcall *)(HGLOBAL)> hGlb(::LoadResource(hInstance, hRsrc), FreeResource);
    if (hGlb == NULL) return false;

    SToolBarData* pTBData = (SToolBarData*) ::LockResource(hGlb.get());
    if (pTBData == NULL) return false;

    assert(pTBData->wVersion == 1);

    std::vector<TBBUTTON> Buttons(pTBData->wItemCount, {});

    int Image = 0;
    for (int I = 0; I < pTBData->wItemCount; ++I)
    {
        TBBUTTON *ThisButton = &Buttons[I];
        ThisButton->fsState = TBSTATE_ENABLED;
        ThisButton->idCommand = pTBData->items()[I];
        if (ThisButton->idCommand == 0)
        {
            ThisButton->fsStyle = TBSTYLE_SEP;
            if (Style & TBSTYLE_FLAT)
                ThisButton->iBitmap = 6;
            else
                ThisButton->iBitmap = 8;
        }
        else
        {
            ThisButton->fsStyle = TBSTYLE_BUTTON;
            ThisButton->iBitmap = Image++;
        }
    }

    Create(Parent, Style, ID, pTBData->wItemCount, hInstance, ResID, Buttons.data(),
        pTBData->wItemCount, pTBData->wWidth, pTBData->wHeight, 0, 0);

    UnlockResource(hGlb.get()); // Doesn;t do anything so it is exception safe

    return true;
}
