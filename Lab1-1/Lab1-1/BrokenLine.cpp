#include "stdafx.h"
#include "BrokenLine.h"

void BrokenLine::Draw(HDC hdc)
{
	if (count > 0)
	{
		HPEN oldPen;

		//qColor = RGB(0, 0, 0);
		hPen = CreatePen(PS_SOLID, iBrushSize, qColor);
		oldPen = (HPEN)SelectObject(hdc, hPen);//store old pen
		MoveToEx(hdc, aPoint[0].x, aPoint[0].y, NULL);
		for (int i = 1; i < count - 1; i++)
		{
			LineTo(hdc, aPoint[i].x, aPoint[i].y);
		}
		LineTo(hdc, aPoint[count - 1].x, aPoint[count - 1].y);
		SelectObject(hdc, oldPen);
		DeleteObject(hPen);
	}
}
