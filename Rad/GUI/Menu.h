#pragma once

#include <Windows.h>

#include "WindowProxy.h"
#include "..\Win\RadObject.h"

namespace rad
{
    class MenuRef;
    using Menu = rad::Owner<MenuRef>;

    class MenuRef : public Object<HMENU>
    {
    public:
        using Object<HMENU>::Object;

        void CheckItem(int Item, bool Check, bool ByPos)
        {
            UINT Flags = Check ? MF_CHECKED : MF_UNCHECKED;
            Flags |= ByPos ? MF_BYPOSITION : MF_BYCOMMAND;
            ::CheckMenuItem(GetHandle(), Item, Flags);
        }

        void CheckRadioItem(int First, int Last, int Item, bool ByPos)
        {
            UINT Flags = ByPos ? MF_BYPOSITION : MF_BYCOMMAND;
            ::CheckMenuRadioItem(GetHandle(), First, Last, Item, Flags);
        }

        void EnableItem(int Item, bool Enable, bool ByPos)
        {
            UINT Flags = Enable ? MF_ENABLED : MF_GRAYED;
            Flags |= ByPos ? MF_BYPOSITION : MF_BYCOMMAND;
            ::EnableMenuItem(GetHandle(), Item, Flags);
        }

        void GetInfo(LPMENUINFO MenuInfo) const
        {
            if (GetMenuInfo(GetHandle(), MenuInfo) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void GetItemInfo(int ID, LPMENUITEMINFO MenuItemInfo, bool ByPos) const
        {
            if (GetMenuItemInfo(GetHandle(), ID, ByPos, MenuItemInfo) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        int GetItemString(int ID, LPTSTR String, int MaxChar, bool ByPos) const
        {
            UINT	Flag = ByPos ? MF_BYPOSITION : MF_BYCOMMAND;
            int	RetVal = GetMenuString(GetHandle(), ID, String, MaxChar, Flag);
            if (RetVal == 0)
                ThrowWinError(_T(__FUNCTION__));
            return RetVal;
        }

        MenuRef GetSubMenu(int Pos) const
        {
            assert(IsValid());
            return ::GetSubMenu(GetHandle(), Pos);
        }

        void SetInfo(LPMENUINFO MenuInfo)
        {
            if (SetMenuInfo(GetHandle(), MenuInfo) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void SetItemInfo(int ID, LPMENUITEMINFO MenuItemInfo, bool ByPos)
        {
            if (SetMenuItemInfo(GetHandle(), ID, ByPos, MenuItemInfo) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void Append(UINT ID, const char *String, UINT Flags = 0)
        {
            assert(IsValid());
            assert((Flags & MF_POPUP) == 0);
            Flags |= MF_STRING;
            if (::AppendMenuA(GetHandle(), Flags, ID, String) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void Append(UINT ID, const wchar_t *String, UINT Flags = 0)
        {
            assert(IsValid());
            assert((Flags & MF_POPUP) == 0);
            Flags |= MF_STRING;
            if (::AppendMenuW(GetHandle(), Flags, ID, String) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void Append(Menu& NewMenu, const char *String, UINT Flags = 0);

        void Append(Menu& NewMenu, const wchar_t *String, UINT Flags = 0);

        void AppendSeparator(UINT Flags = 0)
        {
            assert(IsValid());
            assert((Flags & MF_POPUP) == 0);
            assert((Flags & MF_STRING) == 0);
            Flags |= MF_SEPARATOR;
            if (::AppendMenu(GetHandle(), Flags, (UINT_PTR) -1, 0) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void Delete(UINT ID, UINT Flags = 0)
        {
            assert(IsValid());
            if (::DeleteMenu(GetHandle(), ID, Flags) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void Insert(int Pos, UINT ID, const TCHAR *String, UINT Flags = 0)
        {
            assert(IsValid());
            assert((Flags & MF_POPUP) == 0);
            Flags |= MF_STRING;
            if (::InsertMenu(GetHandle(), Pos, Flags, ID, String) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void Insert(int Pos, MenuRef NewMenu, const TCHAR *String, UINT Flags = 0)
        {
            assert(IsValid());
            Flags |= MF_POPUP;
            Flags |= MF_STRING;
            if (::InsertMenu(GetHandle(), Pos, Flags, (UINT_PTR) NewMenu.GetHandle(), String) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void InsertSeparator(int Pos, UINT Flags = 0)
        {
            assert(IsValid());
            assert((Flags & MF_POPUP) == 0);
            assert((Flags & MF_STRING) == 0);
            Flags |= MF_SEPARATOR;
            if (::InsertMenu(GetHandle(), Pos, Flags, 0, 0) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        int GetItemCount() const
        {
            return GetMenuItemCount(GetHandle());
        }

        UINT GetItemID(int Item) const
        {
            return GetMenuItemID(GetHandle(), Item);
        }

        UINT GetItemState(int Item, bool ByPos) const
        {
            return GetMenuState(GetHandle(), Item, ByPos ? MF_BYPOSITION : MF_BYCOMMAND);
        }

        void Modify(UINT Pos, UINT Flags, UINT ID, const TCHAR *Name)
        {
            assert(IsValid());
            assert((Flags & MF_POPUP) == 0);
            Flags |= MF_STRING;
            if (::ModifyMenu(GetHandle(), Pos, Flags, ID, Name) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void Modify(UINT Pos, UINT Flags, MenuRef NewMenu, const TCHAR *Name)
        {
            assert(IsValid());
            Flags |= MF_POPUP;
            Flags |= MF_STRING;
            if (::ModifyMenu(GetHandle(), Pos, Flags, (UINT_PTR) NewMenu.GetHandle(), Name) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void SetDefaultItem(UINT Item, bool ByPos)
        {
            assert(IsValid());
            if (::SetMenuDefaultItem(GetHandle(), Item, ByPos) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        int TrackPopup(UINT Flags, int X, int Y, WindowProxy Wnd)
        {
            return TrackPopupMenu(GetHandle(), Flags, X, Y, 0, Wnd.GetHWND(), NULL);
        }

        int GetDefaultItem(bool ByPos, UINT Flags = 0)
        {
            return GetMenuDefaultItem(GetHandle(), ByPos ? TRUE : FALSE, Flags);
        }

        void Delete() override
        {
            if (::DestroyMenu(GetHandle()) == 0)
                ThrowWinError(_T(__FUNCTION__));
            Release();
        }
    };

    inline void MenuRef::Append(Menu& NewMenu, const char *String, UINT Flags)
    {
        assert(IsValid());
        Flags |= MF_POPUP;
        Flags |= MF_STRING;
        if (::AppendMenuA(GetHandle(), Flags, (UINT_PTR) NewMenu.GetHandle(), String) == 0)
            ThrowWinError(_T(__FUNCTION__));
        NewMenu.Release();
    }

    inline void MenuRef::Append(Menu& NewMenu, const wchar_t *String, UINT Flags)
    {
        assert(IsValid());
        Flags |= MF_POPUP;
        Flags |= MF_STRING;
        if (::AppendMenuW(GetHandle(), Flags, (UINT_PTR) NewMenu.GetHandle(), String) == 0)
            ThrowWinError(_T(__FUNCTION__));
        NewMenu.Release();
    }
}
