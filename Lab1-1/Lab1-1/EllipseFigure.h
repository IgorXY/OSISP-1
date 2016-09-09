#pragma once
#include "AbstractFigure.h"
#include "Lab1-1.h"

class EllipseFigure : public AbstractFigure
{
	virtual void Draw(HDC hdc) override;
};