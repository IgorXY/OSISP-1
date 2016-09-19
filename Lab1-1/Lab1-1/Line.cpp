
#include "stdafx.h"
#include "Line.h"

void Line::Draw(HDC hdc, double scale)
{
	
	HPEN oldPen;
	//qColor = RGB(0, 0, 0);
	hPen = CreatePen(PS_SOLID, iBrushSize*scale, qColor);
	oldPen = (HPEN)SelectObject(hdc, hPen);//store old pen

	MoveToEx(hdc, x1*scale, y1*scale, NULL);
	LineTo(hdc, x2*scale, y2*scale);

	SelectObject(hdc, oldPen);
	DeleteObject(hPen);

}

void Line::Pan(POINT p)
{
	x1 += p.x;
	x2 += p.x;
	y1 += p.y;
	y2 += p.y;
}
