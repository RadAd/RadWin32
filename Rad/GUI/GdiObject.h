#ifndef __GDIOBJECT_H__
#define __GDIOBJECT_H__

#include <commctrl.h>
#include "DevContext.H"
#include "..\Win\RadObject.h"

namespace rad
{
    class GDIObjectRef : public Object<HGDIOBJ>
    {
    public:
        using Object<HGDIOBJ>::Object;

        void Delete() override
        {
            if (IsValid())
            {
                if (!::DeleteObject(GetHandle()))
                    ThrowWinError(_T(__FUNCTION__));
                Release();
            }
        }

        void GetObject(LPVOID Details, int Length) const
        {
            if (::GetObject(GetHandle(), Length, Details) == 0)
                ThrowWinError(_T(__FUNCTION__));
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

    class BrushRef : public GDIObjectRef
    {
    public:
        using GDIObjectRef::GDIObjectRef;

        HBRUSH GetBrushHandle() const
        {
            return (HBRUSH) GetHandle();
        }

        void GetObject(LOGBRUSH* Details) const
        {
            GDIObjectRef::GetObject((void*) Details, sizeof(LOGBRUSH));
        }
    };

    class Brush : public BrushRef
    {
    public:
        Brush(HBRUSH Object = NULL)
            : BrushRef(Object)
        {
        }

        Brush(const Brush&) = delete;

        virtual void Detach() override
        {
            Delete();
        }

        void Create(const LOGBRUSH* LogBrush)
        {
            Attach(CreateBrushIndirect(LogBrush));
        }

        void Create(COLORREF Color)
        {
            Attach(CreateSolidBrush(Color));
        }
    };

    class PenRef : public GDIObjectRef
    {
    public:
        using GDIObjectRef::GDIObjectRef;

        void GetObject(LOGPEN* Details) const
        {
            GDIObjectRef::GetObject((void*) Details, sizeof(LOGPEN));
        }
    };

    class Pen : public PenRef
    {
    public:
        Pen(HPEN Object = NULL)
            : PenRef(Object)
        {
        }

        Pen(const Pen&) = delete;

        virtual void Detach() override
        {
            Delete();
        }

        void Create(COLORREF Color, int Width = 0, int Style = PS_SOLID)
        {
            Attach(CreatePen(Style, Width, Color));
        }

        HPEN GetPenHandle() const
        {
            return (HPEN) GetHandle();
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

    class FontRef : public GDIObjectRef
    {
    public:
        using GDIObjectRef::GDIObjectRef;

        HFONT GetFontHandle() const
        {
            return (HFONT) GetHandle();
        }

        void GetObject(LOGFONT* Details) const
        {
            GDIObjectRef::GetObject((void*) Details, sizeof(LOGFONT));
        }
    };

    class Font : public FontRef
    {
    public:
        Font(HFONT Object = NULL)
            : FontRef(Object)
        {
        }

        Font(const Font&) = delete;

        virtual void Detach() override
        {
            Delete();
        }

        void Create(const LOGFONT* LogFont)
        {
            m_LogFont = LogFont;
            Attach(CreateFontIndirect(LogFont));
        }

        const LOGFONT* GetLogFont() const
        {
            return &m_LogFont;
        }

    private:
        LogFont     m_LogFont;
    };

    class BitmapRef : public GDIObjectRef
    {
    public:
        using GDIObjectRef::GDIObjectRef;

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
            GDIObjectRef::GetObject((void*) Details, sizeof(BITMAP));
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

    class Bitmap : public Owner<BitmapRef>
    {
    public:
        using Owner<BitmapRef>::Owner;

        virtual void Detach() override
        {
            Delete();
        }

        void Create(DevContextRef DC, int Width, int Height)
        {
            Attach(CreateCompatibleBitmap(DC.GetHandle(), Width, Height));
            assert(IsValid());
        }

        void Create(const BITMAP* Bitmap)
        {
            Attach(CreateBitmapIndirect(Bitmap));
            assert(IsValid());
        }

        void CreateDIB(DevContextRef DC, const BITMAPINFO* bi, void* bits, UINT usage)
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
    };

    class TempSelectObject
    {
    public:
        TempSelectObject(DevContextRef DC, GDIObjectRef _GDIObject)
            : m_DC(DC)
        {
            assert(m_DC.IsValid());
            m_OldGDIObject = ::SelectObject(m_DC.GetHandle(), _GDIObject.GetHandle());
        }
        ~TempSelectObject()
        {
            ::SelectObject(m_DC.GetHandle(), m_OldGDIObject.GetHandle());
        }

    private:
        DevContextRef   m_DC;
        GDIObjectRef    m_OldGDIObject;
    };

    class PaletteRef : public GDIObjectRef
    {
    public:
        using GDIObjectRef::GDIObjectRef;

        HPALETTE GetPaletteHandle() const
        {
            return (HPALETTE) GetHandle();
        }

        void GetObject(LOGPALETTE* Details) const
        {
            GDIObjectRef::GetObject((void*) Details, sizeof(LOGPALETTE));
        }
    };

    class Palette : public PaletteRef
    {
    public:
        Palette(HPALETTE Object = NULL)
            : PaletteRef(Object)
        {
        }

        Palette(const Palette&) = delete;

        virtual void Detach() override
        {
            Delete();
        }

        void Create(const LOGPALETTE* LogPalette)
        {
            Attach(CreatePalette(LogPalette));
        }
    };

    // Icon isn't actually a GDI object
    class IconRef : public Object<HICON>
    {
    public:
        using Object<HICON>::Object;

        void Delete() override
        {
            if (!DestroyIcon(GetHandle()))
                ThrowWinError(_T(__FUNCTION__));
            Release();
        }
    };

    class Icon : public Owner<IconRef>
    {
    public:
        using Owner<IconRef>::Owner;

        void Create(_In_opt_ HINSTANCE hInstance,
            _In_ int nWidth,
            _In_ int nHeight,
            _In_ BYTE cPlanes,
            _In_ BYTE cBitsPixel,
            _In_ CONST BYTE *lpbANDbits,
            _In_ CONST BYTE *lpbXORbits)
        {
            Attach(CreateIcon(hInstance,
                nWidth,
                nHeight,
                cPlanes,
                cBitsPixel,
                lpbANDbits,
                lpbXORbits));
            assert(IsValid());
        }
    };
}

#endif
