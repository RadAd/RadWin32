#pragma once

#include <Rad\GUI\WindowProxy.h>

#include <windowsx.h>

namespace rad
{
    // TODO
    // StaticWnd
    // ButtonWnd
    // EditWnd
    // ScrollBarWnd
    // ListBox

    class ComboBoxWnd : public WindowProxy
    {
    public:
        using WindowProxy::WindowProxy;

        ComboBoxWnd()
        {
        }

        ComboBoxWnd(const WindowProxy& Other)
            : WindowProxy(Other)
        {
        }

        int LimitText(int cchLimit) { return ComboBox_LimitText(GetHWND(), cchLimit); }

        DWORD GetEditSel()const { return ComboBox_GetEditSel(GetHWND()); }
        int SetEditSel(int ichStart, int ichEnd) { return ComboBox_SetEditSel(GetHWND(), ichStart, ichEnd); }

        int GetCount() const { return ComboBox_GetCount(GetHWND()); }
        int ResetContent() { return ComboBox_ResetContent(GetHWND()); }

        int AddString(LPCTSTR lpsz) { return ComboBox_AddString(GetHWND(), lpsz); }
        int InsertString(int index, LPCTSTR lpsz) { return ComboBox_InsertString(GetHWND(), index, lpsz); }

        int AddItemData(LPARAM data) { return ComboBox_AddItemData(GetHWND(), data); }
        int InsertItemData(int index, LPARAM data) { return ComboBox_InsertItemData(GetHWND(), index, data); }

        int DeleteString(int index) { return ComboBox_DeleteString(GetHWND(), index); }

        int GetLBTextLen(int index) const { return ComboBox_GetLBTextLen(GetHWND(), index); }
        int GetLBText(int index, LPCTSTR lpszBuffer) { return ComboBox_GetLBText(GetHWND(), index, lpszBuffer); }

        LRESULT GetItemData(int index) const { return ComboBox_GetItemData(GetHWND(), index); }
        int SetItemData(int index, LPARAM data) { return ComboBox_SetItemData(GetHWND(), index, data); }

        int FindString(int indexStart, LPCTSTR lpszFind) const { return ComboBox_FindString(GetHWND(), indexStart, lpszFind); }
        int FindItemData(int indexStart, LPARAM data) const { return ComboBox_FindItemData(GetHWND(), indexStart, data); }

        int GetCurSel() const { return ComboBox_GetCurSel(GetHWND()); }
        int SetCurSel(int index) { return ComboBox_SetCurSel(GetHWND(), index); }

        int SelectString(int indexStart, LPCTSTR lpszSelect) { return ComboBox_SelectString(GetHWND(), indexStart, lpszSelect); }
        int SelectItemData(int indexStart, LPARAM data) { return ComboBox_SelectItemData(GetHWND(), indexStart, data); }

        int Dir(UINT attrs, LPCTSTR lpszFileSpec) { return ComboBox_Dir(GetHWND(), attrs, lpszFileSpec); }

        BOOL ShowDropdown(BOOL fShow) { return ComboBox_ShowDropdown(GetHWND(), fShow); }

#if (WINVER >= 0x030a)
        int FindStringExact(int indexStart, LPCTSTR lpszFind) const { return ComboBox_FindStringExact(GetHWND(), indexStart, lpszFind); }

        BOOL GetDroppedState() const { return ComboBox_GetDroppedState(GetHWND()); }
        void GetDroppedControlRect(RECT* lprc) const { ComboBox_GetDroppedControlRect(GetHWND(), lprc); }

        int GetItemHeight() const { return ComboBox_GetItemHeight(GetHWND()); }
        int SetItemHeight(int index, int cyItem) { return ComboBox_SetItemHeight(GetHWND(), index, cyItem); }

        UINT GetExtendedUI() { return ComboBox_GetExtendedUI(GetHWND()); }
        int SetExtendedUI(UINT flags) { return ComboBox_SetExtendedUI(GetHWND(), flags); }
#endif  /* WINVER >= 0x030a */
    };

}
