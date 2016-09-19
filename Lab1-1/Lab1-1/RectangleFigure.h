#pragma once
#include "Line.h"
#include "Lab1-1.h"

class RectangleFigure : public Line
{
	virtual void Draw(HDC hdc, double scale) override;
	virtual void Pan(POINT p) override;
};
