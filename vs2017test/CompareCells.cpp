#include "CompareCells.h"

extern int pacManRow, pacManCol;
extern int targetRow, targetCol;

bool CompareCellsAStar::compareCells(Cell* pc1, Cell* pc2)
{
	int horDisCell1 = abs(pacManCol - pc1->GetCol());
	int vertDisCell1 = abs(pacManRow - pc1->GetRow());
	int horDisCell2 = abs(pacManCol - pc2->GetCol());
	int vertDisCell2 = abs(pacManRow - pc2->GetRow());
	return horDisCell1 + vertDisCell1 + pc1->GetG() > horDisCell2 + vertDisCell2 + pc2->GetG();
}
bool CompareCellsAStar::operator()(Cell* pc1, Cell* pc2)
{
	{
		return compareCells(pc1, pc2);

	}
}

bool CompareCellsPacManAStar::compareCells(Cell* pc1, Cell* pc2)
{
	int horDisCell1 = abs(targetCol - pc1->GetCol());
	int vertDisCell1 = abs(targetRow - pc1->GetRow());
	int horDisCell2 = abs(targetCol - pc2->GetCol());
	int vertDisCell2 = abs(targetRow - pc2->GetRow());
	int cell_1_Distance = horDisCell1 + vertDisCell1, cell_2_Distance = horDisCell2 + vertDisCell2;

	return cell_1_Distance + pc1->GetG() - 4 * pc1->GetCoin_value() > cell_2_Distance + pc2->GetG() - 4 * pc2->GetCoin_value();
}

bool CompareCellsPacManAStar::operator()(Cell* pc1, Cell* pc2)
{
	return compareCells(pc1, pc2);
}
