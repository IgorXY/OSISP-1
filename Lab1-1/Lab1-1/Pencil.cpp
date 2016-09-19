#include "stdafx.h"
#include "Pencil.h"

//void Pensil::Draw()
//{
//	hwnd
//}

void Pencil::Draw(HDC hdc, double scale)
{
	if (count > 0)
	{
		HPEN oldPen;
		
		//qColor = RGB(0, 0, 0);
		hPen = CreatePen(PS_SOLID, iBrushSize*scale, qColor);
		oldPen = (HPEN)SelectObject(hdc, hPen);//store old pen
		MoveToEx(hdc, aPoint[0].x*scale, aPoint[0].y*scale, NULL);
		for (int i = 1; i < count-1; i++)
		{
				LineTo(hdc, aPoint[i].x*scale, aPoint[i].y*scale);
		}
		LineTo(hdc, aPoint[count-1].x*scale, aPoint[count-1].y*scale);
		SelectObject(hdc, oldPen);
		DeleteObject(hPen);
	}
}

void Pencil::AddPoint(POINT p)
{
	aPoint = (POINT*)realloc(aPoint, (count + 1) * sizeof(POINT));
	aPoint[count] = p;
	count++;
}

void Pencil::Pan(POINT p)
{
	for (int i = 0; i < count; i++)
	{
		aPoint[i].x += p.x;
		aPoint[i].y += p.y;
	}
}
