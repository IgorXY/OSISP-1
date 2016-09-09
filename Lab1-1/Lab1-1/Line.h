#pragma once
#include "AbstractFigure.h"
#include "Lab1-1.h"

class Line : public AbstractFigure
{
public:
	

	virtual void Draw(HDC hdc) override;
};