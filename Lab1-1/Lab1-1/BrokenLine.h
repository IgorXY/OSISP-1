#pragma once
#include "Pencil.h"
#include "Lab1-1.h"

class BrokenLine : public Pencil
{
public:
	virtual void Draw(HDC hdc, double scale) override;
	virtual void Pan(POINT p) override;
};