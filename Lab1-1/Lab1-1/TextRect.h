#pragma once

#include "AbstractFigure.h"
#include "Lab1-1.h"
#include <string>

class TextRect: public AbstractFigure
{
public:
	std::string str;
	int count = 0;
	void AddChar(char ch);
	void RemoveChar();

	virtual void Draw(HDC hdc) override;
};