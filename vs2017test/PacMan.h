
#include <iostream>
#include <queue>
#include <stack>
#include "CompareCells.h"
#include "Cell.h"
#include "glut.h"
#include <vector>
#include "main.h"
#pragma once

extern int targetRow, targetCol;

class PacMan
{
private:
	int lastManDistance;
	int coins;
	int num_of_monsters;
	int pacMaze[MSZ][MSZ];
	bool faceLeft;
	Cell* pacCell, * monster;
	Cell* target;
	std::queue<Cell*> monsters;
	std::stack<Cell*> path;


	void FindPath(int maze[MSZ][MSZ]);
	void CheckPacmanNeighborAStar(Cell* current, Cell* neighbor, bool* AStar_is_on, std::priority_queue<Cell*, std::vector<Cell*>, CompareCellsPacManAStar>* aStarGrays);
	bool IsMonsterClose(int maze[MSZ][MSZ]);
	void searchMonsters(int maze[MSZ][MSZ]);
	void CheckNeighborBFS(Cell* current, int row, int col, bool* bfs_is_on, std::vector <Cell*>* BFSGrays);
	void RestorePath(Cell* current);
	void copyMaze(int maze[MSZ][MSZ]);
	void find_safe_target(int maze[MSZ][MSZ]);



public:
	//static Cell* currentCell;
	PacMan(Cell* cell);

	int GetCoins()
	{
		return coins;
	};
	int GetCol()
	{
		return pacCell->GetCol();
	};
	int GetRow()
	{
		return pacCell->GetRow();
	};
	void RunAway(int maze[MSZ][MSZ], bool isSearchRequired);
	void PacManStep(int maze[MSZ][MSZ], bool* packman_is_on);
	void DrawPacMan();

};

