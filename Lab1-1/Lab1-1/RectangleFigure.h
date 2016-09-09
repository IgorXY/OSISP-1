#pragma once
#include "Line.h"
#include "Lab1-1.h"

class RectangleFigure : public Line
{
	virtual void Draw(HDC hdc) override;
};
