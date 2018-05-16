#ifndef __GDIOBJECT_H__
#define __GDIOBJECT_H__

#include <commctrl.h>
#include "..\Win\WinHandle.h"
#include "DevContext.H"

namespace rad
{
    inline void CheckCloseGdiObject(HGDIOBJ Object)
    {
        if (Object != NULL)
        {
            if (!::DeleteObject(Object))
                rad::ThrowWinError();
        }
    }

    class GDIObject : private std::unique_ptr<std::remove_pointer<HGDIOBJ>::type, void (*)(HGDIOBJ) >
    {
    public:
        GDIObject(HGDIOBJ Object = NULL)
            : std::unique_ptr<std::remove_pointer<HGDIOBJ>::type, void(*)(HGDIOBJ) >(Object, CheckCloseGdiObject)
        {
        }

        void Attach(HGDIOBJ Object)
        {
            reset(Object);
        }

        HGDIOBJ Release()
        {
            return release();
        }

        void Delete()
        {
            reset();
        }

        bool IsValid() const
        {
            return GetHandle() != NULL;
        }

        HGDIOBJ GetHandle() const
        {
            return get();
        }

        void GetObject(LPVOID Details, int Length) const
        {
            if (::GetObject(GetHandle(), Length, Details) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        explicit operator bool() const _NOEXCEPT
        {	// test for non-null pointer
            return (GetHandle() != NULL);
        }
    };

    class LogBrush : public LOGBRUSH
    {
    public:
        LogBrush()
        {
            ZeroMemory(this, sizeof(LOGBRUSH));
        }

        LogBrush(UINT Style, COLORREF Color, LONG Hatch)
        {
            lbStyle = Style;
            lbColor = Color;
            lbHatch = Hatch;
        }
    };

    class Brush : public GDIObject
    {
    public:
        void Create(const LOGBRUSH* LogBrush)
        {
            Attach(CreateBrushIndirect(LogBrush));
        }

        void Create(COLORREF Color)
        {
            Attach(CreateSolidBrush(Color));
        }

        HBRUSH GetBrushHandle() const
        {
            return (HBRUSH) GetHandle();
        }

        void GetObject(LOGBRUSH* Details) const
        {
            GDIObject::GetObject((void*) Details, sizeof(LOGBRUSH));
        }
    };

    class Pen : public GDIObject
    {
    public:
        void Create(COLORREF Color, int Width = 0, int Style = PS_SOLID)
        {
            Attach(CreatePen(Style, Width, Color));
        }

        HPEN GetPenHandle() const
        {
            return (HPEN) GetHandle();
        }

        void GetObject(LOGPEN* Details) const
        {
            GDIObject::GetObject((void*) Details, sizeof(LOGPEN));
        }
    };

    class LogFont : public LOGFONT
    {
    public:
        LogFont()
        {
            ZeroMemory(this, sizeof(LOGFONT));
            lfHeight = 0;
            lfWidth = 0;
            lfEscapement = 0;
            lfOrientation = 0;
            lfWeight = FW_NORMAL;
            lfItalic = FALSE;
            lfUnderline = FALSE;
            lfStrikeOut = FALSE;
            lfCharSet = DEFAULT_CHARSET;
            lfOutPrecision = OUT_DEFAULT_PRECIS;
            lfClipPrecision = CLIP_DEFAULT_PRECIS;
            lfQuality = DEFAULT_QUALITY;
            lfPitchAndFamily = DEFAULT_PITCH;
            _tcscpy_s(lfFaceName, LF_FACESIZE, TEXT(""));
        }

        void SetFaceName(const TCHAR* FaceName)
        {
            _tcscpy_s(lfFaceName, LF_FACESIZE, FaceName);
        }

        const LogFont& operator=(const LOGFONT* LogFont)
        {
            CopyMemory(this, LogFont, sizeof(LOGFONT));
            return *this;
        }
    };

    class Font : public GDIObject
    {
    public:
        void Create(const LOGFONT* LogFont)
        {
            m_LogFont = LogFont;
            Attach(CreateFontIndirect(LogFont));
        }

        HFONT GetFontHandle() const
        {
            return (HFONT) GetHandle();
        }

        const LOGFONT* GetLogFont() const
        {
            return &m_LogFont;
        }

        void GetObject(LOGFONT* Details) const
        {
            GDIObject::GetObject((void*) Details, sizeof(LOGFONT));
        }

    private:
        LogFont     m_LogFont;
    };

    class Bitmap : public GDIObject
    {
    public:
        void Create(DevContext& DC, int Width, int Height)
        {
            Attach(CreateCompatibleBitmap(DC.GetHandle(), Width, Height));
            assert(IsValid());
        }

        void Create(const BITMAP* Bitmap)
        {
            Attach(CreateBitmapIndirect(Bitmap));
            assert(IsValid());
        }

        void CreateDIB(DevContext& DC, const BITMAPINFO* bi, void* bits, UINT usage)
        {
            Attach(CreateDIBitmap(DC.GetHandle(), &bi->bmiHeader, CBM_INIT, bits, bi, usage));
            assert(IsValid());
        }

        void CreateMapped(HINSTANCE hInstance, int Bitmap, UINT Flags = 0, LPCOLORMAP ColorMap = NULL, int NumMap = 0)
        {
            Attach(CreateMappedBitmap(hInstance, Bitmap, Flags, ColorMap, NumMap));
            assert(IsValid());
        }

        void Load(HINSTANCE hInstance, int Bitmap)
        {
            Load(hInstance, MAKEINTRESOURCE(Bitmap));
        }

        void Load(HINSTANCE hInstance, LPCTSTR Bitmap)
        {
            Attach(::LoadBitmap(hInstance, Bitmap));
            assert(IsValid());
        }

        HBITMAP GetBitmapHandle() const
        {
            return (HBITMAP) GetHandle();
        }

        SIZE GetDimension() const
        {
            SIZE    Size;
            if (GetBitmapDimensionEx((HBITMAP) GetHandle(), &Size) == 0)
                ThrowWinError(_T(__FUNCTION__));
            return Size;
        }

        void GetObject(BITMAP* Details) const
        {
            GDIObject::GetObject((void*) Details, sizeof(BITMAP));
        }

    public: //Utils
        SIZE GetSize() const
        {
            BITMAP bm;
            GetObject(&bm);

            SIZE s;
            s.cx = bm.bmWidth;
            s.cy = bm.bmHeight;
            return s;
        }
    };

    class TempSelectObject
    {
    public:
        TempSelectObject(DevContext& DC, const GDIObject& _GDIObject)
            : m_DC(DC)
        {
            assert(m_DC.IsValid());
            m_OldGDIObject = ::SelectObject(m_DC.GetHandle(), _GDIObject.GetHandle());
        }
        ~TempSelectObject()
        {
            ::SelectObject(m_DC.GetHandle(), m_OldGDIObject);
        }

    private:
        DevContext&     m_DC;
        HGDIOBJ         m_OldGDIObject;
    };
}

#endif
