#pragma once
#include "AbstractFigure.h"
#include "Lab1-1.h"

class Line : public AbstractFigure
{
public:
	virtual void Draw(HDC hdc, double scale) override;
	virtual void Pan(POINT p) override;
};