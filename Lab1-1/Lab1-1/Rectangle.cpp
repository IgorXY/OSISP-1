#include "stdafx.h"
#include "RectangleFigure.h"

void RectangleFigure::Draw(HDC hdc)
{
	HPEN oldPen;
	//qColor = RGB(0, 0, 0);
	hPen = CreatePen(PS_SOLID, 4, qColor);
	oldPen = (HPEN)SelectObject(hdc, hPen);//store old pen

	Rectangle(hdc, x1, y1, x2, y2);

	SelectObject(hdc, oldPen);
	DeleteObject(hPen);
}
