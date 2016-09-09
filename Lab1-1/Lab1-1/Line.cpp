
#include "stdafx.h"
#include "Line.h"

void Line::Draw(HDC hdc)
{
	
	HPEN oldPen;
	//qColor = RGB(0, 0, 0);
	hPen = CreatePen(PS_SOLID, 4, qColor);
	oldPen = (HPEN)SelectObject(hdc, hPen);//store old pen

	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);

	SelectObject(hdc, oldPen);
	DeleteObject(hPen);

}
