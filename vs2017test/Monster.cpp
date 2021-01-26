#include "Monster.h"


using namespace std;




void Monster::copyMaze(int maze[MSZ][MSZ])
{
	int i, j;
	for (i = 0; i < MSZ; i++)
	{
		for (j = 0; j < MSZ; j++)
		{
			monsterMaze[i][j] = maze[i][j];
		}

	}
}

Monster::Monster(Cell* cell, double _r, double _g, double _b, int maze[MSZ][MSZ])
{
	stepsBeforeSearch = MSZ + rand() % MSZ;
	monsterCell = cell;
	red = _r;
	green = _g;
	blue = _b;
	Cell::num_of_Monsters++;
	maze[monsterCell->GetRow()][monsterCell->GetCol()] = COIN_AND_MONSTER;


}

void Monster::updateMaze(int maze[MSZ][MSZ])
{
	if (maze[monsterCell->GetRow()][monsterCell->GetCol()] == COIN)
	{
		maze[monsterCell->GetRow()][monsterCell->GetCol()] = COIN_AND_MONSTER;
	}
	else
	{
		maze[monsterCell->GetRow()][monsterCell->GetCol()] = MONSTER;
	}
}
//A-star
bool Monster::SearchForPac(int maze[MSZ][MSZ])
{
	copyMaze(maze);

	bool hunt_is_on = true;
	Cell* current;
	priority_queue<Cell*, vector <Cell*>, CompareCellsAStar > aStarGrays;
	aStarGrays.push(monsterCell);

	while (hunt_is_on)
	{
		//get cell with minimum(F) from priority queue
		current = aStarGrays.top();
		aStarGrays.pop();
		int currentRow = current->GetRow(), currentCol = current->GetCol();

		//checking each of the four neighbor cells
		CheckMonsterNeighborAStar(current, new Cell(currentRow + 1, currentCol, current), &hunt_is_on, &aStarGrays);
		if (!hunt_is_on)
			return true;
		CheckMonsterNeighborAStar(current, new Cell(currentRow, currentCol + 1, current), &hunt_is_on, &aStarGrays);
		if (!hunt_is_on)
			return true;
		CheckMonsterNeighborAStar(current, new Cell(currentRow - 1, currentCol, current), &hunt_is_on, &aStarGrays);
		if (!hunt_is_on)
			return true;
		CheckMonsterNeighborAStar(current, new Cell(currentRow, currentCol - 1, current), &hunt_is_on, &aStarGrays);
		if (!hunt_is_on)
			return true;

		//once the Cell is out of the queue need to paint it black his path from start not going to change
		if (monsterMaze[currentRow][currentCol] == GRAY)
			monsterMaze[currentRow][currentCol] = BLACK;

		//no more grays to check --> Can't find target
		if (aStarGrays.empty())
		{
			cout << "no solution - SearchForPac\n";
			hunt_is_on = false;
			return false;
		}
	}

}

void Monster::CheckMonsterNeighborAStar(Cell* current, Cell* neighbor, bool* AStar_is_on, std::priority_queue<Cell*, std::vector<Cell*>, CompareCellsAStar>* aStarGrays)
{
	Cell tempNeighbor = *neighbor;
	int neigborRow = neighbor->GetRow(), neighborCol = neighbor->GetCol();

	if (monsterMaze[neigborRow][neighborCol] == PACMAN)
	{

		cout << "Solution Found! - monster a-star\n";
		*AStar_is_on = false;
		RestorePath(neighbor);
		return;
	}

	if (monsterMaze[neigborRow][neighborCol] == SPACE
		|| monsterMaze[neigborRow][neighborCol] == COIN
		|| monsterMaze[neigborRow][neighborCol] == COIN_AND_MONSTER
		|| monsterMaze[neigborRow][neighborCol] == MONSTER)
	{
		neighbor->SetG(current->GetG() + 1);
		neighbor->SetParent(current);
		monsterMaze[neigborRow][neighborCol] = GRAY;
		aStarGrays->push(neighbor);
		return;
	}

	if (monsterMaze[neigborRow][neighborCol] == GRAY)
	{
		tempNeighbor.SetG(current->GetG() + 1);
		if (CompareCellsAStar::compareCells(neighbor, &tempNeighbor))
		{
			neighbor->SetG(tempNeighbor.GetG());
			neighbor->SetParent(current);
		}
		return;
	}



}

void Monster::RestorePath(Cell* current)
{
	path = stack<Cell*>();

	while (current != monsterCell)
	{
		path.push(current);

		current = current->GetParent();
	}
}

void Monster::UpdateMazeBeforeStep(int maze[MSZ][MSZ])
{
	if (maze[monsterCell->GetRow()][monsterCell->GetCol()] == COIN_AND_MONSTER)
	{
		maze[monsterCell->GetRow()][monsterCell->GetCol()] = COIN;
	}

	if (maze[monsterCell->GetRow()][monsterCell->GetCol()] == MONSTER)
	{
		maze[monsterCell->GetRow()][monsterCell->GetCol()] = SPACE;
	}
}

void Monster::UpdateMazeAfterStep(int maze[MSZ][MSZ])
{
	if (maze[monsterCell->GetRow()][monsterCell->GetCol()] == COIN)
	{
		maze[monsterCell->GetRow()][monsterCell->GetCol()] = COIN_AND_MONSTER;
	}

	if (maze[monsterCell->GetRow()][monsterCell->GetCol()] == SPACE)
	{
		maze[monsterCell->GetRow()][monsterCell->GetCol()] = MONSTER;
	}
}

void Monster::MonsterStep(int maze[MSZ][MSZ], bool* pacman_is_on)
{


	if (maze[monsterCell->GetRow()][monsterCell->GetCol()] == PACMAN)
	{
		*pacman_is_on = false;
		return;
	}
	int oldRow = monsterCell->GetRow();
	int oldCol = monsterCell->GetCol();

	if (path.empty())
	{
		SearchForPac(maze);
	}


	UpdateMazeBeforeStep(maze);
	monsterCell = path.top();
	path.pop();
	UpdateMazeAfterStep(maze);
	if (maze[monsterCell->GetRow()][monsterCell->GetCol()] == PACMAN)
	{
		*pacman_is_on = false;
	}


	DrawMonster();

}

void Monster::DrawMonster()
{
	double  theta;
	double width = 2.0 / MSZ;
	double x = 2 * (monsterCell->GetCol() / (double)MSZ) - 1;
	double y = 2 * (monsterCell->GetRow() / (double)MSZ) - 1;

	double 	r = width * 0.4;
	double 	num_segments = 20;
	double 	cx = x + width / 2;
	double 	cy = y + width / 2;

	glColor3d(red, green, blue);

	glBegin(GL_POLYGON);
	for (int ii = 0; ii <= num_segments / 2; ii++)
	{
		theta = 2.0 * PI * ii / num_segments;//get the current angle
		x = r * cos(theta);//calculate the x component
		y = r * sin(theta);//calculate the y component
		glVertex2d(x + cx, y + cy);//output vertex
	}
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2d(cx - r, cy);
	glVertex2d(cx - r / 2, cy - r);
	glVertex2d(cx, cy);
	glEnd();

	glColor3d(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2d(cx - r, cy);
	glVertex2d(cx - r / 2, cy - r);
	glVertex2d(cx, cy);
	glEnd();


	glColor3d(red, green, blue);
	glBegin(GL_POLYGON);
	glVertex2d(cx, cy);
	glVertex2d(cx + r / 2, cy - r);
	glVertex2d(cx + r, cy);
	glEnd();

	glColor3d(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2d(cx, cy);
	glVertex2d(cx + r / 2, cy - r);
	glVertex2d(cx + r, cy);
	glEnd();

	glColor3d(0, 0, 0);
	glBegin(GL_POLYGON);
	for (int ii = 0; ii <= num_segments; ii++)
	{
		theta = 2.0 * PI * ii / num_segments;//get the current angle
		x = 0.2 * r * cos(theta);//calculate the x component
		y = 0.2 * r * sin(theta);//calculate the y component
		glVertex2d(x + cx - r / 3, y + cy + r / 4);//output vertex
	}
	glEnd();

	glBegin(GL_POLYGON);
	for (int ii = 0; ii <= num_segments; ii++)
	{
		theta = 2.0 * PI * ii / num_segments;//get the current angle
		x = 0.2 * r * cos(theta);//calculate the x component
		y = 0.2 * r * sin(theta);//calculate the y component
		glVertex2d(x + cx + r / 3, y + cy + r / 4);//output vertex
	}
	glEnd();
}

