#ifndef __GDIOBJECT_H__
#define __GDIOBJECT_H__

#include <commctrl.h>
#include <memory>
#include "DevContext.H"

namespace rad
{
    class GDIObjectDeleter
    {
    public:
        struct pointer
        {
            pointer(HGDIOBJ _Object = NULL, bool _Owns = true)
                : Object(_Object), Owns(_Owns)
            {
            }

            HGDIOBJ Object;
            bool Owns;
        };

        void operator()(pointer Handle)
        {
            if (Handle.Owns && Handle.Object != NULL)
            {
                if (::DeleteObject(Handle.Object) == 0)
                    rad::ThrowWinError();
            }
        }
    };

    bool operator!=(const GDIObjectDeleter::pointer&l, const GDIObjectDeleter::pointer& r)
    {
        return l.Object != r.Object;
    }

    class GDIObject : private std::unique_ptr<GDIObjectDeleter::pointer, GDIObjectDeleter>
    {
    public:
        void Attach(HGDIOBJ Object, bool Owns = true)
        {
            reset(GDIObjectDeleter::pointer(Object, Owns));
        }

        HGDIOBJ Release()
        {
            return release().Object;
        }

        void Delete()
        {
            reset(GDIObjectDeleter::pointer());
        }

        bool IsValid() const
        {
            return GetHandle() != NULL;
        }

        HGDIOBJ GetHandle() const
        {
            return get().Object;
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
            m_DC.SelectObject(&_GDIObject, &m_OldGDIObject);
        }
        ~TempSelectObject()
        {
            m_DC.SelectObject(&m_OldGDIObject);
        }

    private:
        DevContext&     m_DC;
        GDIObject       m_OldGDIObject;
    };

    inline void DevContext::SelectObject(const GDIObject* GDIObj, GDIObject* OldGDIObj)
    {
        HANDLE  Object;
        assert(IsValid());
        Object = ::SelectObject(m_hDC, GDIObj->GetHandle());
        if (OldGDIObj)
            OldGDIObj->Attach(Object, false);
    }
}

#endif
