#pragma once

#include <math.h>


class Cell
{
private:
	int coin_value;
	int row, col, g, h;
	Cell* parent;
public:
	static int num_of_Monsters;
	Cell(int r, int c, Cell* pc);
	int GetRow();
	int GetCol();
	Cell* GetParent();
	int GetG();
	void SetG(int _g);
	void SetParent(Cell* parent);
	void SetCol(int _col)
	{
		col = _col;
	};
	void SetRow(int _row)
	{
		row = _row;
	};
	int GetCoin_value()
	{
		return coin_value;
	}
	void SetCoin_value(int value)
	{
		coin_value = value;
	}
};

