#ifndef __RECT_H__
#define __RECT_H__

// RECT

#include <windef.h>

inline POINT GetTopLeft(const RECT& Rect)
{
    POINT	p = { Rect.left, Rect.top };
    return p;
}

inline POINT GetTopRight(const RECT& Rect)
{
    POINT	p = { Rect.right, Rect.top };
    return p;
}

inline POINT GetBottomLeft(const RECT& Rect)
{
    POINT	p = { Rect.left, Rect.bottom };
    return p;
}

inline POINT GetBottomRight(const RECT& Rect)
{
    POINT	p = { Rect.right, Rect.bottom };
    return p;
}

POINT GetCenter(const RECT& Rect)
{
    POINT	p = { (Rect.left + Rect.right) / 2, (Rect.top + Rect.bottom) / 2 };
    return p;
}

inline LONG GetWidth(const RECT& Rect)
{
    return Rect.right - Rect.left;
}

inline LONG GetHeight(const RECT& Rect)
{
    return Rect.bottom - Rect.top;
}

inline SIZE GetSize(const RECT& Rect)
{
    SIZE	s = { GetWidth(Rect), GetHeight(Rect) };
    return s;
}

inline bool InRect(const POINT& Point, const RECT& Rect)
{
    return ((Point.x >= Rect.left) && (Point.x < Rect.right) &&
        (Point.y >= Rect.top) && (Point.y < Rect.bottom));
}

inline bool InRect(const POINTS& Point, const RECT& Rect)
{
    POINT	pt;
    pt.x = Point.x;
    pt.y = Point.y;
    return InRect(pt, Rect);
}

inline void MoveUp(RECT& Rect, LONG Delta = 1)
{
    Rect.top -= Delta;
    Rect.bottom -= Delta;
}

inline void MoveDown(RECT& Rect, LONG Delta = 1)
{
    Rect.top += Delta;
    Rect.bottom += Delta;
}

inline void MoveRight(RECT& Rect, LONG Delta = 1)
{
    Rect.left += Delta;
    Rect.right += Delta;
}

inline void MoveLeft(RECT& Rect, LONG Delta = 1)
{
    Rect.left -= Delta;
    Rect.right -= Delta;
}

inline bool IncreaseHeight(RECT& Rect, LONG Delta = 1)
{
    Rect.bottom += Delta;;
    return true;
}

inline bool DecreaseHeight(RECT& Rect, LONG Delta = 1)
{
    if (Rect.bottom > Rect.top)
    {
        Rect.bottom -= Delta;;
        return true;
    }
    else
    {
        return false;
    }
}

inline bool IncreaseWidth(RECT& Rect, LONG Delta = 1)
{
    Rect.right += Delta;;
    return true;
}

inline bool DecreaseWidth(RECT& Rect, LONG Delta = 1)
{
    if (Rect.right > Rect.left)
    {
        Rect.right -= Delta;
        return true;
    }
    else
    {
        return false;
    }
}

inline void Inflate(RECT& Rect, LONG DeltaX, LONG DeltaY)
{
    Rect.top -= DeltaX;
    Rect.bottom += DeltaX;
    Rect.left -= DeltaY;
    Rect.right += DeltaY;
}

inline RECT GetRect(const POINT& pt, const SIZE& size)
{
    RECT	rc;
    rc.top = pt.y;
    rc.bottom = pt.y + size.cy;
    rc.left = pt.x;
    rc.right = pt.x + size.cx;
    return rc;
}

#endif
