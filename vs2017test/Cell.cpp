#include "Cell.h"
#include <stdlib.h>
#pragma once

int Cell::num_of_Monsters = 0;

Cell::Cell(int r, int c, Cell* pc)
{
	row = r;
	col = c;
	parent = pc;
	g = 0;
	coin_value = 1;
	//coin_value = rand() % 2 + 1;
}

int Cell::GetRow()
{
	return row;
}

int Cell::GetCol()
{
	return col;
}

Cell* Cell::GetParent()
{
	return parent;
}

int Cell::GetG()
{
	return g;
}

void Cell::SetG(int _g)
{
	g = _g;
}

void Cell::SetParent(Cell* _parent)
{
	parent = _parent;
}
