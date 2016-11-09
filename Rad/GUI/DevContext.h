#ifndef __DEVCONTEXT_H__
#define __DEVCONTEXT_H__

#include "WindowProxy.H"

namespace rad
{
    class GDIObject;        // see "GdiObject.H"

    class DevContext
    {
    public:
        static const HDC Invalid;

        DevContext(HDC hDC = Invalid)
            : m_hDC(hDC)
        {
        }

        virtual ~DevContext()
        {
            //Delete();    // Can't do it here, can't call a virtual funtion in the destructor, must be added to derived classes
        }

        virtual void Delete()
        {
        }

        void Attach(HDC hDC)
        {
            Delete();
            m_hDC = hDC;
        }

        HDC Release()
        {
            HDC hDC = m_hDC;
            m_hDC = Invalid;
            return hDC;
        }

    public:
        bool IsValid() const
        {
            return m_hDC != Invalid;
        }

        HDC GetHandle() const
        {
            return m_hDC;
        }

        void Box(int Top, int Left, int Bottom, int Right)
        {
            MoveToEx(Left, Top);
            LineTo(Right, Top);
            LineTo(Right, Bottom);
            LineTo(Left, Bottom);
            LineTo(Left, Top);
        }

        void AlphaBlend(int XDest, int YDest, int Width, int Height, const DevContext &SrDevContext, int XSrc, int YSrc, int WidthSrc, int HeightSrc, BLENDFUNCTION ftn)
        {
            assert(IsValid());
            if (::AlphaBlend(m_hDC, XDest, YDest, Width, Height, SrDevContext.m_hDC, XSrc, YSrc, WidthSrc, HeightSrc, ftn) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void BitBlt(int XDest, int YDest, int Width, int Height, const DevContext &SrDevContext, int XSrc, int YSrc, DWORD RasterOp)
        {
            assert(IsValid());
            if (::BitBlt(m_hDC, XDest, YDest, Width, Height, SrDevContext.m_hDC, XSrc, YSrc, RasterOp) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void RealizePalette()
        {
            assert(IsValid());
            if (::RealizePalette(m_hDC) == GDI_ERROR)
                ThrowWinError(_T(__FUNCTION__));
        }

        void StretchBlt(int XDest, int YDest, int WidthDest, int HeightDest, const DevContext &SrDevContext, int XSrc, int YSrc, int WidthSrc, int HeightSrc, DWORD RasterOp)
        {
            assert(IsValid());
            if (::StretchBlt(m_hDC, XDest, YDest, WidthDest, HeightDest, SrDevContext.m_hDC, XSrc, YSrc, WidthSrc, HeightSrc, RasterOp) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void TransparentBlt(int XDest, int YDest, int Width, int Height, const DevContext &SrDevContext, int XSrc, int YSrc, int WidthSrc, int HeightSrc, COLORREF BgColor)
        {
            assert(IsValid());
            if (::TransparentBlt(m_hDC, XDest, YDest, Width, Height, SrDevContext.m_hDC, XSrc, YSrc, WidthSrc, HeightSrc, BgColor) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void DrawFrameControl(LPRECT Rect, UINT Type, UINT State)
        {
            assert(IsValid());
            if (::DrawFrameControl(m_hDC, Rect, Type, State) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void DrawIcon(int x, int y, HICON icon)
        {
            assert(IsValid());
            if (::DrawIcon(m_hDC, x, y, icon) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void DrawIconEx(int xLeft, int yTop,
            HICON hIcon, int cxWidth, int cyWidth,
            UINT istepIfAniCur, HBRUSH hbrFlickerFreeDraw, UINT diFlags)
        {
            assert(IsValid());
            if (::DrawIconEx(m_hDC, xLeft, yTop,
                hIcon, cxWidth, cyWidth,
                istepIfAniCur, hbrFlickerFreeDraw, diFlags) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void DrawTextA(LPCSTR String, int Count, LPRECT Rect, UINT Format)
        {
            assert(IsValid());
            if (::DrawTextA(m_hDC, String, Count, Rect, Format) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void DrawTextW(LPCWSTR String, int Count, LPRECT Rect, UINT Format)
        {
            assert(IsValid());
            if (::DrawTextW(m_hDC, String, Count, Rect, Format) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void FillRect(const LPRECT Rect, HBRUSH Brush)
        {
            assert(IsValid());
            if (::FillRect(m_hDC, Rect, Brush) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        COLORREF GetPixel(int X, int Y)
        {
            COLORREF Color = ::GetPixel(GetHandle(), X, Y);
            if (Color == CLR_INVALID)
                ThrowWinError(_T(__FUNCTION__));
            return Color;
        }

        void GetTextMetrics(LPTEXTMETRIC TM) const
        {
            if (::GetTextMetrics(GetHandle(), TM) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void LineTo(int X, int Y)
        {
            if (::LineTo(GetHandle(), X, Y) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        POINT MoveToEx(int X, int Y)
        {
            POINT LastPoint;
            if (::MoveToEx(GetHandle(), X, Y, &LastPoint) == 0)
                ThrowWinError(_T(__FUNCTION__));
            return LastPoint;
        }

        void PaintDesktop()
        {
            if (::PaintDesktop(GetHandle()) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void Polygon(const LPPOINT points, int count)
        {
            if (count > 0)
                if (::Polygon(GetHandle(), points, count) == 0)
                    ThrowWinError(_T(__FUNCTION__));
        }

        void Polyline(const LPPOINT points, int count)
        {
            if (count > 0)
                if (::Polyline(GetHandle(), points, count) == 0)
                    ThrowWinError(_T(__FUNCTION__));
        }

        void Rectangle(int nLeftRect, int nTopRect, int nRightRect, int nBottomRect)
        {
            if (::Rectangle(GetHandle(), nLeftRect, nTopRect, nRightRect, nBottomRect) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void PlayEnhMetaFile(HENHMETAFILE hEmf, const LPRECT Rect)
        {
            assert(IsValid());
            if (::PlayEnhMetaFile(m_hDC, hEmf, Rect) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        void DevContext::SelectObject(const GDIObject* GDIObj, GDIObject* OldGDIObj = nullptr);    // see "GdiObject.H"
    /*
        HGDIOBJ SelectObject(HGDIOBJ hGDIObj)
        {
            assert(IsValid());
            return ::SelectObject(m_hDC, hGDIObj);
        }
    */
        void SetBkMode(int Mode)
        {
            if (::SetBkMode(GetHandle(), Mode) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

        COLORREF SetBkColor(COLORREF Color)
        {
            COLORREF OldColor = ::SetBkColor(GetHandle(), Color);
            if (OldColor == CLR_INVALID)
                ThrowWinError(_T(__FUNCTION__));
            return OldColor;
        }

        COLORREF SetPixel(int X, int Y, COLORREF Color)
        {
            COLORREF OldColor = ::SetPixel(GetHandle(), X, Y, Color);
            if (OldColor == CLR_INVALID)
                ThrowWinError(_T(__FUNCTION__));
            return OldColor;
        }

        void SetPixelV(int X, int Y, COLORREF Color)
        {
            BOOL    Valid = ::SetPixelV(GetHandle(), X, Y, Color);
            if (Valid == FALSE)
                ThrowWinError(_T(__FUNCTION__));
        }

        COLORREF SetTextColor(COLORREF Color)
        {
            COLORREF OldColor = ::SetTextColor(GetHandle(), Color);
            if (OldColor == CLR_INVALID)
                ThrowWinError(_T(__FUNCTION__));
            return OldColor;
        }

        void TextOut(int X, int Y, LPCTSTR Text, int Count)
        {
            if (::TextOut(GetHandle(), X, Y, Text, Count) == 0)
                ThrowWinError(_T(__FUNCTION__));
        }

    private:
        HDC    m_hDC;
    };

    class CPaintDC : public DevContext
    {
    public:
        CPaintDC()
        {
        }

        CPaintDC(WindowProxy &Window)
        {
            Create(Window);
        }

        ~CPaintDC()
        {
            Delete();
        }

        void Create(WindowProxy &Window)
        {
            m_Window = Window;
            Attach(BeginPaint(m_Window.GetHWND(), &m_ps));
            if (!IsValid())
                ThrowWinError(_T(__FUNCTION__));
        }

        void Delete()
        {
            if (IsValid())
            {
                EndPaint(m_Window.GetHWND(), &m_ps);
                Release();
            }
        }

        const PAINTSTRUCT* GetPaintStruct() const
        {
            return &m_ps;
        }

    private:
        PAINTSTRUCT    m_ps;
        WindowProxy    m_Window;
    };

    class CWindowDC : public DevContext
    {
    public:
        CWindowDC()
        {
        }

        CWindowDC(WindowProxy &Window)
        {
            Create(Window);
        }

        ~CWindowDC()
        {
            Delete();
        }

        void Create(WindowProxy &Window)
        {
            m_Window = Window;
            Attach(::GetDC(m_Window.GetHWND()));
            if (!IsValid())
                ThrowWinError(_T(__FUNCTION__));
        }

        void Delete()
        {
            if (IsValid())
            {
                ::ReleaseDC(m_Window.GetHWND(), Release());
            }
        }

    private:
        WindowProxy    m_Window;
    };

    class CMemDC : public DevContext
    {
    public:
        CMemDC()
        {
        }

        CMemDC(DevContext& OtherDevContext)
        {
            Create(OtherDevContext);
        }

        ~CMemDC()
        {
            Delete();
        }

        void Create(DevContext& OtherDevContext)
        {
            Attach(::CreateCompatibleDC(OtherDevContext.GetHandle()));
            if (!IsValid())
                ThrowWinError(_T(__FUNCTION__));
        }

        void Delete()
        {
            if (IsValid())
            {
                ::DeleteDC(Release());
            }
        }

    private:
    };
}

#endif
