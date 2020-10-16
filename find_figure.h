#pragma once

#include <iostream>
#define figure_size 15

class figure
{
public:
	figure(const int (&picture)[figure_size][figure_size])
	{
		for (size_t i = 1; i < figure_size - 1; i++)
		{
			for (size_t j = 1; j < figure_size - 1; j++)
				this->picture[i][j] = picture[i][j];
		}
	}
	
	enum figure_type { undefined, square, circle };
	void PrintFigure();
	void DefineFigure();
	void GetData();
	size_t GetNumberOfCharacters();

private:
	int picture[figure_size][figure_size] = { 0 };
	size_t	diameter = 0,
			length_of_side = 0,
			column = 0, 
			string = 0;
	figure_type test = undefined;
	figure_type DefineSquare();
	figure_type DefineCircle();
};

