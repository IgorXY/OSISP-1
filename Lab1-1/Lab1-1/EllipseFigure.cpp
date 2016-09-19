#include "stdafx.h"
#include "EllipseFigure.h"

void EllipseFigure::Draw(HDC hdc, double scale)
{
	HPEN oldPen;
	HBRUSH oldBrush;
	//qColor = RGB(0, 0, 0);
	hPen = CreatePen(PS_SOLID, iBrushSize*scale, qColor);
	hBrush = CreateSolidBrush(qColor2);
	oldPen = (HPEN)SelectObject(hdc, hPen);//store old pen
	oldBrush = (HBRUSH)SelectObject(hdc, hBrush);//store old pen

	Ellipse(hdc, x1*scale, y1*scale, x2*scale, y2*scale);

	SelectObject(hdc, oldPen);
	SelectObject(hdc, oldBrush);
	DeleteObject(hPen);
}

void EllipseFigure::Pan(POINT p)
{
	x1 += p.x;
	x2 += p.x;
	y1 += p.y;
	y2 += p.y;
}

