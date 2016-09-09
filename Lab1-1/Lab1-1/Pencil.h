#pragma once

#include "AbstractFigure.h"
#include "Lab1-1.h"

class Pencil: public AbstractFigure
{
	// Inherited via AbstractFigure
public:
	POINT *aPoint = NULL;
	int count = 0;

	void AddPoint(POINT p);

	virtual void Draw(HDC hdc) override;
};