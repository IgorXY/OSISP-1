#include "stdafx.h"
#include "PrintRect.h"

void PrintRect::Draw(HDC hdc, double scale)
{
	HPEN oldPen;
	HBRUSH oldBrush;
	//qColor = RGB(0, 0, 0);
	hPen = CreatePen(PS_DOT, iBrushSize*scale, qColor);
	oldPen = (HPEN)SelectObject(hdc, hPen);//store old pen
	oldBrush = (HBRUSH)SelectObject(hdc, hBrush);//store old pen
	SelectObject(hdc, GetStockObject(NULL_BRUSH));
	Rectangle(hdc, x1*scale, y1*scale, x2*scale, y2*scale);

	SelectObject(hdc, oldPen);
	SelectObject(hdc, oldBrush);
	DeleteObject(hPen);
}

void PrintRect::Pan(POINT p)
{
}
