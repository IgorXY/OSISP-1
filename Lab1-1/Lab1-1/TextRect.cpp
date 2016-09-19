#include "stdafx.h"
#include "TextRect.h"

void TextRect::AddChar(char ch)
{
	str += ch;
	count++;
}

void TextRect::RemoveChar()
{
	if (count > 0)
	{
		count--;
		str.pop_back();
	}
}

void TextRect::Draw(HDC hdc, double scale)
{
	if (count > 0) {
		HPEN oldPen;
		//qColor = RGB(0, 0, 0);
		hPen = CreatePen(PS_SOLID, iBrushSize*scale, qColor);
		oldPen = (HPEN)SelectObject(hdc, hPen);//store old pen

		TextOutA(hdc, x1*scale, y1*scale, str.c_str(), strlen(str.c_str()));

		SelectObject(hdc, oldPen);
		DeleteObject(hPen);
	}
}

void TextRect::Pan(POINT p)
{
	x1 += p.x;
	y1 += p.y;
}

