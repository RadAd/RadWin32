#ifndef __COMMCTRL_H__
#define __COMMCTRL_H__

#include "WindowProxy.h"
#include "..\Win\RadObject.h"

#include <CommCtrl.h>
#include <windowsx.h>

namespace rad
{
    class Icon;

    class StatusWnd : public WindowProxy
    {
    public:
        using WindowProxy::WindowProxy;

        void Create(const WindowProxy &Parent, DWORD Style, UINT ID, const TCHAR *Text = NULL)
        {
            Attach(
                CreateStatusWindow(Style, Text, Parent.GetHWND(), ID)
            );
        }

        void SetText(const TCHAR *Text, int Type, int Part = 0)
        {
            SendMessage(SB_SETTEXT, Type | Part, (LPARAM) Text);
        }

        int GetTextLength(int Part = 0, int *Style = NULL)
        {
            int RetVal = (int) SendMessage(SB_GETTEXTLENGTH, Part);
            if (Style)
                *Style = HIWORD(RetVal);
            return LOWORD(RetVal);
        }

        void SetParts(int Count, int *Widths)
        {
            if (SendMessage(SB_SETPARTS, (WPARAM) Count, (LPARAM) Widths) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }
    };

    class ToolBarWnd : public WindowProxy
    {
    public:
        using WindowProxy::WindowProxy;

        void Create(const WindowProxy &Parent, DWORD Style, UINT ID, int Bitmaps, HINSTANCE hBMInstance, UINT BMID,
            LPCTBBUTTON Buttons, int NumButtons, int dxButton, int dyButton, int dxBitmap, int dyBitmap)
        {
            Attach(
                CreateToolbarEx(Parent.GetHWND(), Style, ID, Bitmaps, hBMInstance, BMID,
                    Buttons, NumButtons, dxButton, dyButton, dxBitmap, dyBitmap, sizeof(TBBUTTON))
            );
        }

        void Create256(const WindowProxy &Parent, DWORD Style, UINT ID, HINSTANCE hBMInstance, UINT BMID,
            LPCTBBUTTON Buttons, int NumButtons, int dxButton, int dyButton)
        {
            // https://msdn.microsoft.com/en-us/library/windows/desktop/hh298381(v=vs.85).aspx
            Attach(
                CreateWindowEx(0, TOOLBARCLASSNAME, NULL, Style, 0, 0, 0, 0, Parent.GetHWND(), (HMENU) (INT_PTR) ID, hBMInstance, NULL)
            );

            if (hBMInstance == HINST_COMMCTRL)
            {
                HIMAGELIST hImageList = ImageList_Create(dxButton, dyButton, ILC_COLOR32 | ILC_MASK, NumButtons, 0);

                SetImageList(hImageList);
                SendMessage(TB_LOADIMAGES, (WPARAM) BMID, (LPARAM) hBMInstance);
            }
            else
            {
                HBITMAP hBmp = LoadBitmap(hBMInstance, MAKEINTRESOURCE(BMID));
                COLORREF RgbMask = RGB(192, 192, 192); // TODO Get top left pixel from image

                HIMAGELIST hImageList = ImageList_Create(dxButton, dyButton, ILC_COLOR32 | ILC_MASK, NumButtons, 0);
                ImageList_AddMasked(hImageList, hBmp, RgbMask);

                SetImageList(hImageList);
            }

            AddButtons(Buttons, NumButtons);
            AutoSize();
        }

        bool Create(const WindowProxy &Parent, DWORD Style, UINT ID, HINSTANCE hInstance, UINT ResID);

        void AutoSize()
        {
            SendMessage(TB_AUTOSIZE);
        }

        DWORD SetExtendedStyle(DWORD dwExStyle)
        {
            return (DWORD) SendMessage(TB_SETEXTENDEDSTYLE, 0, dwExStyle);
        }

        void AddButtons(const TBBUTTON* Buttons, int NumButtons)
        {
            SendMessage(TB_BUTTONSTRUCTSIZE, (WPARAM) sizeof(TBBUTTON));
            if (SendMessage(TB_ADDBUTTONS, (WPARAM) NumButtons, (LPARAM) Buttons) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        int GetButtonCount() const
        {
            return (int) SendMessage(TB_BUTTONCOUNT);
        }

        void GetButton(int Index, LPTBBUTTON Button) const
        {
            if (SendMessage(TB_GETBUTTON, Index, (LPARAM) Button) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        int GetState(UINT ID) const
        {
            return (int) SendMessage(TB_GETSTATE, ID);
        }

        int GetButtonInfo(UINT ID, LPTBBUTTONINFO bi) const
        {
            return (int) SendMessage(TB_GETBUTTONINFO, ID, (LPARAM) bi);
        }

        HIMAGELIST GetImageList() const
        {
            return (HIMAGELIST) SendMessage(TB_GETIMAGELIST);
        }

        HIMAGELIST SetImageList(HIMAGELIST hImgList, int id = 0) const
        {
            return (HIMAGELIST) SendMessage(TB_SETIMAGELIST, id, (LPARAM) hImgList);
        }

        int CommandToIndex(UINT ID) const
        {
            return (int) SendMessage(TB_COMMANDTOINDEX, ID);
        }

        int GetBitmapIndex(UINT ID) const
        {
            if (CommandToIndex(ID) >= 0)
                return (int) SendMessage(TB_GETBITMAP, ID);
            else
                return -1;
        }

        SIZE GetButtonSize() const
        {
            DWORD	WordSize = (DWORD) SendMessage(TB_GETBUTTONSIZE);
            SIZE	Size;
            Size.cx = LOWORD(WordSize);
            Size.cy = HIWORD(WordSize);
            return Size;
        }

        void SetButtonInfo(UINT ID, LPTBBUTTONINFO bi)
        {
            if (SendMessage(TB_SETBUTTONINFO, ID, (LPARAM) bi) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void SetState(UINT ID, int State) const
        {
            if (SendMessage(TB_SETSTATE, ID, MAKELONG(State, 0)) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }
    };

    class ToolTipWnd : public WindowProxy
    {
    public:
        using WindowProxy::WindowProxy;

        bool Create(const WindowProxy& Parent, DWORD Flags = TTS_ALWAYSTIP)
        {
            HINSTANCE hinstMyDll = GetModuleHandle(TEXT("comctl32.dll"));

            Attach(CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
                WS_POPUP | TTS_NOPREFIX | Flags,
                CW_USEDEFAULT, CW_USEDEFAULT,
                CW_USEDEFAULT, CW_USEDEFAULT,
                Parent.GetHWND(), NULL, hinstMyDll,
                NULL));

            ::SetWindowPos(GetHWND(), HWND_TOPMOST, 0, 0, 0, 0,
                SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

            return IsWindow();
        }

        void Activate(bool active = true)
        {
            SendMessage(TTM_ACTIVATE, (WPARAM) (BOOL) active);
        }

        void SetMaxTipWidth(int width)
        {
            SendMessage(TTM_SETMAXTIPWIDTH, 0, (LPARAM) width);
        }

        void AddTool(const TTTOOLINFOW* ti)
        {
            if (SendMessage(TTM_ADDTOOLW, 0, (LPARAM) ti) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void AddTool(const TTTOOLINFOA* ti)
        {
            if (SendMessage(TTM_ADDTOOLA, 0, (LPARAM) ti) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void DelTool(const TTTOOLINFOW* ti)
        {
            SendMessage(TTM_DELTOOLW, 0, (LPARAM) ti);
        }

        void DelTool(const TTTOOLINFOA* ti)
        {
            SendMessage(TTM_DELTOOLA, 0, (LPARAM) ti);
        }

        bool EnumTools(int id, TOOLINFO* ti)
        {
            return SendMessage(TTM_ENUMTOOLS, id, (LPARAM) ti) != FALSE;
        }

        int GetToolCount() const
        {
            return (int) SendMessage(TTM_GETTOOLCOUNT);
        }

        void SetTitle(const char* text, int icon = 0)
        {
            SendMessage(TTM_SETTITLEA, (WPARAM) icon, (LPARAM) text);
        }

        void SetTitle(const wchar_t* text, int icon = 0)
        {
            SendMessage(TTM_SETTITLEW, (WPARAM) icon, (LPARAM) text);
        }

        void Update()
        {
            SendMessage(TTM_UPDATE);
        }

        bool HitTest(LPHITTESTINFO lphti)
        {
            return SendMessage(TTM_HITTEST, 0, (LPARAM) lphti) != FALSE;
        }
    };

    class TreeViewWnd : public WindowProxy
    {
    public:
        using WindowProxy::WindowProxy;

        bool Create(const WindowProxy& Parent, DWORD id, const RECT& WindowRect, DWORD Flags = TTS_ALWAYSTIP)
        {
            HINSTANCE hinstMyDll = GetModuleHandle(TEXT("comctl32.dll"));

            Attach(CreateWindowEx(0, WC_TREEVIEW, NULL,
                WS_VISIBLE | WS_CHILD | WS_BORDER | Flags,
                WindowRect.left, WindowRect.top,
                WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top,
                Parent.GetHWND(), (HMENU) (UINT_PTR) id, hinstMyDll,
                NULL));

#if 0
            ::SetWindowPos(GetHWND(), HWND_TOPMOST, 0, 0, 0, 0,
                SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
#endif

            return IsWindow();
        }

        HTREEITEM GetChild(HTREEITEM hItem)
        {
            return TreeView_GetChild(GetHWND(), hItem);
        }

        HTREEITEM GetSiblingNext(HTREEITEM hItem)
        {
            return TreeView_GetNextSibling(GetHWND(), hItem);
        }

        HTREEITEM GetSiblingPrev(HTREEITEM hItem)
        {
            return TreeView_GetPrevSibling(GetHWND(), hItem);
        }

        HTREEITEM GetParent(HTREEITEM hItem)
        {
            return TreeView_GetParent(GetHWND(), hItem);
        }

        HTREEITEM GetRoot()
        {
            return TreeView_GetRoot(GetHWND());
        }

        BOOL GetItemRect(HTREEITEM hitem, LPRECT prc, BOOL fItemRect)
        {
            return TreeView_GetItemRect(GetHWND(), hitem, prc, fItemRect);
        }

        HTREEITEM GetNextItem(HTREEITEM hItem, UINT flag)
        {
            return TreeView_GetNextItem(GetHWND(), hItem, flag);
        }

        HTREEITEM GetDropHilight()
        {
            return TreeView_GetDropHilight(GetHWND());
        }

        HTREEITEM GetSelection()
        {
            return TreeView_GetSelection(GetHWND());
        }

        BOOL GetItem(LPTVITEM pitem)
        {
            return TreeView_GetItem(GetHWND(), pitem);
        }

        BOOL SetItem(LPTVITEM pitem)
        {
            return TreeView_SetItem(GetHWND(), pitem);
        }

        BOOL DeleteItem(HTREEITEM hItem)
        {
            return TreeView_DeleteItem(GetHWND(), hItem);
        }

        HWND EditLabel(HTREEITEM hItem)
        {
            return TreeView_EditLabel(GetHWND(), hItem);
        }

        HWND GetEditControl()
        {
            return TreeView_GetEditControl(GetHWND());
        }

        HTREEITEM HitTest(LPTVHITTESTINFO lpht)
        {
            return TreeView_HitTest(GetHWND(), lpht);
        }

        HTREEITEM InsertItem(LPTVINSERTSTRUCT lpis)
        {
            return TreeView_InsertItem(GetHWND(), lpis);
        }

        BOOL Expand(HTREEITEM hTreeItem, UINT flag)
        {
            return TreeView_Expand(GetHWND(), hTreeItem, flag);
        }

        HIMAGELIST SetImageList(HIMAGELIST himl, int iImage)
        {
            return TreeView_SetImageList(GetHWND(), himl, iImage);
        }

        BOOL SelectDropTarget(HTREEITEM hItem)
        {
            return TreeView_SelectDropTarget(GetHWND(), hItem);
        }

        BOOL SelectItem(HTREEITEM hItem)
        {
            return TreeView_SelectItem(GetHWND(), hItem);
        }

        BOOL SortChildrenCB(LPTVSORTCB psort, BOOL fRecurse)
        {
            return TreeView_SortChildrenCB(GetHWND(), psort, fRecurse);
        }

        HTREEITEM QuickHitTest(POINT pt, UINT flags)    // TODO Should flags be returned ?
        {
            TVHITTESTINFO hti;
            hti.pt = pt;

            HTREEITEM hItem = HitTest(&hti);

            if (!(hti.flags & flags))
                hItem = NULL;

            return hItem;
        }

        LPARAM QuickGetParam(HTREEITEM hItem)
        {
            if (hItem == TVI_ROOT)
                return 0;

            TVITEM item = {};
            item.hItem = hItem;
            item.mask = TVIF_PARAM;
            GetItem(&item);
            return item.lParam;
        }
    };

    class TabCtrlWnd : public WindowProxy
    {
    public:
        using WindowProxy::WindowProxy;

        bool Create(const WindowProxy& Parent, DWORD id, const RECT& WindowRect, DWORD Flags = TTS_ALWAYSTIP)
        {
            HINSTANCE hinstMyDll = GetModuleHandle(TEXT("comctl32.dll"));

            Attach(CreateWindowEx(0, WC_TABCONTROL, NULL,
                WS_VISIBLE | WS_CHILD | Flags,
                WindowRect.left, WindowRect.top,
                WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top,
                Parent.GetHWND(), (HMENU) (UINT_PTR) id, hinstMyDll,
                NULL));

#if 0
            ::SetWindowPos(GetHWND(), HWND_TOPMOST, 0, 0, 0, 0,
                SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
#endif

            return IsWindow();
        }

        void AdjustRect(RECT* r, BOOL f) const
        {
            TabCtrl_AdjustRect(GetHWND(), f, r);
        }

        void DeleteItem(int i)
        {
            if (TabCtrl_DeleteItem(GetHWND(), i) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void GetItem(int i, TC_ITEM * pItem) const
        {
            if (!TabCtrl_GetItem(GetHWND(), i, pItem))
                ThrowWinError(_T(__FUNCTION__));
        }

        void SetItem(int i, const TC_ITEM * pItem)
        {
            if (!TabCtrl_SetItem(GetHWND(), i, pItem))
                ThrowWinError(_T(__FUNCTION__));
        }

        int GetItemCount() const
        {
            return TabCtrl_GetItemCount(GetHWND());
        }

        int GetCurSel() const
        {
            return TabCtrl_GetCurSel(GetHWND());
        }

        int SetCurSel(int i) const
        {
            return TabCtrl_SetCurSel(GetHWND(), i);
        }

        int InsertItem(int i, const TCITEM* item)
        {
            return TabCtrl_InsertItem(GetHWND(), i, item);
        }

        LPARAM GetParam(int i) const
        {
            TC_ITEM item = {};
            item.mask = TCIF_PARAM;
            GetItem(i, &item);
            return item.lParam;
        }

        int FindParam(LPARAM lParam)
        {
            int count = GetItemCount();
            for (int i = 0; i < count; ++i)
            {
                TC_ITEM item = {};
                item.mask = TCIF_PARAM;
                GetItem(i, &item);
                if (item.lParam == lParam)
                    return i;
            }
            return -1;
        }
    };

    class ImageListRef : public Object<HIMAGELIST>
    {
    public:
        using Object<HIMAGELIST>::Object;

        void Delete() override
        {
            if (!::ImageList_Destroy(GetHandle()))
                ThrowWinError(_T(__FUNCTION__));
            Release();
        }

    public:
        SIZE GetIconSize() const
        {
            int cx, cy;
            if (!ImageList_GetIconSize(GetHandle(), &cx, &cy))
                ThrowWinError(_T(__FUNCTION__));
            SIZE sz;
            sz.cx = cx;
            sz.cy = cy;
            return sz;
        }

        Icon GetIcon(_In_ int i, _In_ UINT flags) const;
    };

    class ImageList : public Owner<ImageListRef>
    {
    public:
        using  Owner<ImageListRef>::Owner;

        void Create(int cx, int cy, UINT flags, int cInitial, int cGrow)
        {
            Attach(ImageList_Create(cx, cy, flags, cInitial, cGrow));
            assert(IsValid());
        }
    };

}

#endif
