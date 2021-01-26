#pragma once
#include "Cell.h"



class CompareCellsAStar
{
public:
	static bool compareCells(Cell* pc1, Cell* pc2);

	bool operator() (Cell* pc1, Cell* pc2);

};

class CompareCellsPacManAStar
{
public:
	static bool compareCells(Cell* pc1, Cell* pc2);

	bool operator() (Cell* pc1, Cell* pc2);

};

