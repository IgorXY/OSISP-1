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

void TextRect::Draw(HDC hdc)
{
	if (count > 0) {
		HPEN oldPen;
		//qColor = RGB(0, 0, 0);
		hPen = CreatePen(PS_SOLID, 4, qColor);
		oldPen = (HPEN)SelectObject(hdc, hPen);//store old pen

		TextOutA(hdc, x1, y1, str.c_str(), strlen(str.c_str()));

		SelectObject(hdc, oldPen);
		DeleteObject(hPen);
	}
}
