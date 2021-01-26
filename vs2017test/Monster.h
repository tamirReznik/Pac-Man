#include "main.h"
#include "PacMan.h"
#include <queue>
#include <stack>
#include <iostream>
#include "CompareCells.h"
#include "Cell.h"
#include "glut.h"
#pragma once

class Monster
{
private:

	std::stack<Cell*> path;
	Cell* monsterCell;
	double red, green, blue;
	int monsterMaze[MSZ][MSZ];
	int stepsBeforeSearch;
	void copyMaze(int maze[MSZ][MSZ]);
	void UpdateMazeBeforeStep(int maze[MSZ][MSZ]);
	void UpdateMazeAfterStep(int maze[MSZ][MSZ]);

public:


	Monster(Cell* Cell, double _r, double _g, double _b, int maze[MSZ][MSZ]);
	bool SearchForPac(int maze[MSZ][MSZ]);
	void CheckMonsterNeighborAStar(Cell* current, Cell* neighbor, bool* AStar_is_on, std::priority_queue<Cell*, std::vector <Cell*>, CompareCellsAStar >* aStarGrays);
	void RestorePath(Cell* current);
	void updateMaze(int maze[MSZ][MSZ]);
	void MonsterStep(int maze[MSZ][MSZ], bool* pacman_is_on);
	void DrawMonster();
	Cell* GetCell()
	{
		return monsterCell;
	};
	int GetsStepsBeforeSearch()
	{
		return stepsBeforeSearch;
	};
};
