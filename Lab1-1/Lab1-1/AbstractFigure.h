#pragma once

class AbstractFigure
{
public:
	int x1, y1, x2, y2;
	HPEN hPen;
	COLORREF qColor;

	virtual void Draw(HDC hdc) = 0;
};
