#pragma once
#include "RectangleFigure.h"
#include "Lab1-1.h"

class PrintRect : public RectangleFigure
{
	virtual void Draw(HDC hdc, double scale) override;
	virtual void Pan(POINT p) override;
};
