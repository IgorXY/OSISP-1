#include "stdafx.h"
#include "EllipseFigure.h"

void EllipseFigure::Draw(HDC hdc)
{
	HPEN oldPen;
	HBRUSH oldBrush;
	//qColor = RGB(0, 0, 0);
	hPen = CreatePen(PS_SOLID, 4, qColor);
	hBrush = CreateSolidBrush(qColor2);
	oldPen = (HPEN)SelectObject(hdc, hPen);//store old pen
	oldBrush = (HBRUSH)SelectObject(hdc, hBrush);//store old pen

	Ellipse(hdc, x1, y1, x2, y2);

	SelectObject(hdc, oldPen);
	SelectObject(hdc, oldBrush);
	DeleteObject(hPen);
}
