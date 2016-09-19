#pragma once

class AbstractFigure
{
public:
	int x1, y1, x2, y2, iBrushSize;
	HPEN hPen;
	HBRUSH hBrush;
	COLORREF qColor, qColor2;

	virtual void Draw(HDC hdc, double scale) = 0;
	virtual void Pan(POINT p) = 0;
};
