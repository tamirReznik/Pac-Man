#include "PacMan.h"
#include "Monster.h"
using namespace std;

extern int pacManRow, pacManCol;
int targetRow, targetCol;

PacMan::PacMan(Cell* cell)
{
	lastManDistance = MSZ / 5;
	coins = 0;
	pacCell = cell;
	num_of_monsters = Cell::num_of_Monsters;
}

void PacMan::RunAway(int maze[MSZ][MSZ], bool isSearchRequired)
{


	path = std::stack<Cell*>();
	if (isSearchRequired)
	{
		searchMonsters(maze);
	}
	target = new Cell(-1, -1, nullptr);

	//create_escape_path(target, maze);
	find_safe_target(maze);
	if (target->GetCol() > -1 && target->GetCol() > -1)
	{
		//run astar on target
		FindPath(maze);
	}

}

//BFS
void PacMan::searchMonsters(int maze[MSZ][MSZ])
{
	copyMaze(maze);

	bool bfs_is_on = true;
	Cell* current;
	int curr_row, curr_col;
	monster = nullptr;
	vector <Cell*> BFSGrays;
	BFSGrays.push_back(pacCell);

	while (bfs_is_on)
	{
		if (BFSGrays.empty()) // no more grays....no solution
		{

			cout << "pac-man can't locate monster (BFS)\n";
			bfs_is_on = false;
		}
		else
		{
			// take the first element, paint it black and add to grays its white neighbors
			current = BFSGrays.front();
			BFSGrays.erase(BFSGrays.begin()); // remove the front element
			curr_row = current->GetRow();
			curr_col = current->GetCol();
			// paint it black
			if (pacMaze[curr_row][curr_col] == GRAY)
			{
				pacMaze[curr_row][curr_col] = BLACK;
			}
			// look for white neighbors

			// up
			CheckNeighborBFS(current, curr_row + 1, curr_col, &bfs_is_on, &BFSGrays);

			// right
			CheckNeighborBFS(current, curr_row, curr_col + 1, &bfs_is_on, &BFSGrays);

			// left
			CheckNeighborBFS(current, curr_row, curr_col - 1, &bfs_is_on, &BFSGrays);

			// down
			CheckNeighborBFS(current, curr_row - 1, curr_col, &bfs_is_on, &BFSGrays);


		}
	}//while


}

void PacMan::CheckNeighborBFS(Cell* current, int row, int col, bool* bfs_is_on, std::vector <Cell*>* BFSGrays)
{

	if (pacMaze[row][col] == MONSTER
		|| pacMaze[row][col] == COIN_AND_MONSTER)
	{
		monster = new Cell(row, col, nullptr);
		pacMaze[row][col] = GRAY;
		*bfs_is_on = false;

	}
	else
	{
		if (pacMaze[row][col] == SPACE
			|| pacMaze[row][col] == COIN) // paint it gray
		{
			pacMaze[row][col] = GRAY;
			BFSGrays->push_back(new Cell(row, col, current));
		}
	}
}

void PacMan::RestorePath(Cell* current)
{
	path = stack<Cell*>();
	while (current != pacCell)
	{
		path.push(current);

		current = current->GetParent();
	}
}

void PacMan::copyMaze(int maze[MSZ][MSZ])
{
	int i, j;
	for (i = 0; i < MSZ; i++)
	{
		for (j = 0; j < MSZ; j++)
		{
			pacMaze[i][j] = maze[i][j];
		}
	}
}

int compute_head_value()
{
	return MSZ - 1 - MSZ / 4 + (rand() % (MSZ / 4));
}

int compute_tail_value()
{
	return rand() % (MSZ / 4) + 1;
}

/*
* ### finding a safe target for the next path of pac-man ###
*
* Calculating target row and col:
* the manhattan distance between Pac-man and monster determined the destination of Pac-man
* If the distance between the monster and Pacman is greater than MSZ, then necessarily the monster is not in the same quarter as Pacman in the maze. In that case, Pac-man will try to stay at this distance or further until the monsters get close.
* If the distance between the monster and Pacman is less than MSZ, Pacman's target would be one of the closest quarters.
*/
void PacMan::find_safe_target(int maze[MSZ][MSZ])
{
	int col = 0, row = 0;
	int pacCol = pacCell->GetCol(), pacRow = pacCell->GetRow();
	int monsterCol = monster->GetCol(), monsterRow = monster->GetRow();
	int closeDist = 5;
	int (*headValue)(void);
	int (*tailValue)(void);
	int   v_dist = pacRow - monsterRow, h_dist = pacCol - monsterCol;

	if (abs(h_dist) + abs(v_dist) > MSZ + MSZ / 2)
	{

		headValue = &compute_head_value;
		tailValue = &compute_tail_value;
	}
	else
	{
		tailValue = &compute_head_value;
		headValue = &compute_tail_value;
	}

	if (monsterCol >= MSZ / 2 && monsterRow >= MSZ / 2)
	{



		if (h_dist <= closeDist && v_dist <= 0 || v_dist<closeDist && h_dist > closeDist)
		{
			col = pacCell->GetCol();
			do
			{
				row = (*headValue)();
			} while (maze[row][col] != SPACE && maze[row][col] != COIN && maze[row][col] != MONSTER && maze[row][col] != COIN_AND_MONSTER);
		}
		else
		{

			row = pacCell->GetRow();
			do
			{
				col = (*headValue)();
			} while (maze[row][col] != SPACE && maze[row][col] != COIN && maze[row][col] != MONSTER && maze[row][col] != COIN_AND_MONSTER);
		}

		//	col = pacCell->GetCol();
		//	row = (*headValue)();

	}

	if (monsterCol >= MSZ / 2 && monsterRow <= MSZ / 2)
	{

		//	row = pacCell->GetRow();
			//col = (*headValue)();


		if (h_dist < closeDist && v_dist >= 0 || v_dist > -closeDist && h_dist > closeDist)
		{
			col = pacCell->GetCol();
			do
			{
				row = (*tailValue)();
			} while (maze[row][col] != SPACE && maze[row][col] != COIN && maze[row][col] != MONSTER && maze[row][col] != COIN_AND_MONSTER);
		}
		else
		{

			row = pacCell->GetRow();
			do
			{
				col = (*headValue)();
			} while (maze[row][col] != SPACE && maze[row][col] != COIN && maze[row][col] != MONSTER && maze[row][col] != COIN_AND_MONSTER);
		}




	}
	if (monsterCol <= MSZ / 2 && monsterRow <= MSZ / 2)
	{

		//col = pacCell->GetCol();
		//row = (*tailValue)();

		if (h_dist >= -closeDist && v_dist >= 0 || v_dist > -closeDist && h_dist < -closeDist)
		{
			col = pacCell->GetCol();
			do
			{
				row = (*tailValue)();
			} while (maze[row][col] != SPACE && maze[row][col] != COIN && maze[row][col] != MONSTER && maze[row][col] != COIN_AND_MONSTER);
		}
		else
		{

			row = pacCell->GetRow();
			do
			{
				col = (*tailValue)();
			} while (maze[row][col] != SPACE && maze[row][col] != COIN && maze[row][col] != MONSTER && maze[row][col] != COIN_AND_MONSTER);
		}


	}

	if (monsterCol <= MSZ / 2 && monsterRow >= MSZ / 2)
	{

		//col = (*tailValue)();
		//row = pacCell->GetRow();
		if (h_dist >= -closeDist && v_dist <= 0 || v_dist < closeDist && h_dist < -closeDist)
		{
			col = pacCell->GetCol();
			do
			{
				row = (*headValue)();
			} while (maze[row][col] != SPACE && maze[row][col] != COIN && maze[row][col] != MONSTER && maze[row][col] != COIN_AND_MONSTER);
		}
		else
		{

			row = pacCell->GetRow();
			do
			{
				col = (*tailValue)();
			} while (maze[row][col] != SPACE && maze[row][col] != COIN && maze[row][col] != MONSTER && maze[row][col] != COIN_AND_MONSTER);
		}



	}
	target->SetCol(col);
	target->SetRow(row);
	targetCol = col;
	targetRow = row;

}

//A-STAR
void PacMan::FindPath(int maze[MSZ][MSZ])
{

	copyMaze(maze);
	pacMaze[target->GetRow()][target->GetCol()] = SAFE_TARGET;

	bool aStar_is_on = true;
	Cell* current;
	priority_queue<Cell*, vector <Cell*>, CompareCellsPacManAStar > aStarGrays;
	aStarGrays.push(pacCell);

	while (aStar_is_on)
	{
		//get cell with minimum(F) from priority queue
		current = aStarGrays.top();
		aStarGrays.pop();
		int currentRow = current->GetRow(), currentCol = current->GetCol();

		//checking each of the four neighbor cells
		CheckPacmanNeighborAStar(current, new Cell(currentRow + 1, currentCol, current), &aStar_is_on, &aStarGrays);
		if (!aStar_is_on)
			return;
		CheckPacmanNeighborAStar(current, new Cell(currentRow, currentCol + 1, current), &aStar_is_on, &aStarGrays);
		if (!aStar_is_on)
			return;
		CheckPacmanNeighborAStar(current, new Cell(currentRow - 1, currentCol, current), &aStar_is_on, &aStarGrays);
		if (!aStar_is_on)
			return;
		CheckPacmanNeighborAStar(current, new Cell(currentRow, currentCol - 1, current), &aStar_is_on, &aStarGrays);
		if (!aStar_is_on)
			return;

		//once the Cell is out of the queue need to paint it black his path from start not going to change
		if (pacMaze[currentRow][currentCol] == GRAY)
			pacMaze[currentRow][currentCol] = BLACK;

		//no more grays to check --> Can't find target
		if (aStarGrays.empty())
		{
			cout << "no solution - FindPath pac-man\n";
			aStar_is_on = false;
		}
	}

}

void PacMan::CheckPacmanNeighborAStar(Cell* current, Cell* neighbor, bool* AStar_is_on, std::priority_queue<Cell*, std::vector<Cell*>, CompareCellsPacManAStar>* aStarGrays)
{
	Cell tempNeighbor = *neighbor;

	if (pacMaze[neighbor->GetRow()][neighbor->GetCol()] == SPACE)
	{
		neighbor->SetCoin_value(0);
	}
	if (pacMaze[neighbor->GetRow()][neighbor->GetCol()] == SAFE_TARGET)
	{

		cout << "Solution Found! pac-man a-star\n";
		*AStar_is_on = false;
		RestorePath(neighbor);
		return;
	}


	if (pacMaze[neighbor->GetRow()][neighbor->GetCol()] == GRAY)
	{
		tempNeighbor.SetG(current->GetG() + 1);
		if (CompareCellsAStar::compareCells(neighbor, &tempNeighbor))
		{
			neighbor->SetG(tempNeighbor.GetG());
			neighbor->SetParent(current);
		}
		return;
	}


	if (pacMaze[neighbor->GetRow()][neighbor->GetCol()] == SPACE
		|| pacMaze[neighbor->GetRow()][neighbor->GetCol()] == COIN
		|| pacMaze[neighbor->GetRow()][neighbor->GetCol()] == COIN_AND_MONSTER
		|| pacMaze[neighbor->GetRow()][neighbor->GetCol()] == MONSTER)
	{
		neighbor->SetG(current->GetG() + 1);
		neighbor->SetParent(current);
		pacMaze[neighbor->GetRow()][neighbor->GetCol()] = GRAY;
		aStarGrays->push(neighbor);
	}
}

//if the closest monster is less then (manhattan distance) MSZ/5 (initial value) && less then last distance recorded from monster then path must be recalculate
bool PacMan::IsMonsterClose(int maze[MSZ][MSZ])
{

	searchMonsters(maze);
	int monsterCol = monster->GetCol();
	int monsterRow = monster->GetRow();
	int manDistance = abs(monsterRow - pacCell->GetRow()) + abs(monsterCol - pacCell->GetCol());

	if (manDistance < lastManDistance)
	{
		lastManDistance = manDistance;
		return true;
	}
	else
	{
		if (manDistance > MSZ / 5)
		{
			lastManDistance = MSZ / 5;
		}
		return false;
	}

}

void PacMan::PacManStep(int maze[MSZ][MSZ], bool* pacman_is_on)
{

	Cell* oldPac = pacCell;
	bool isMonsterClose = IsMonsterClose(maze);

	if (path.empty() || isMonsterClose)
	{
		RunAway(maze, !isMonsterClose);//2nd param - isSearchRequired => if monsterClose then the monster queue Updated accordingly in IsMonsterClose method
	}

	//safe target could be block by wall arround him that is why !path.empty() is required 
	if (!path.empty())
	{
		pacCell = path.top();
		path.pop();
	}


	//update static indicator of pacman location 
	pacManCol = pacCell->GetCol();
	pacManRow = pacCell->GetRow();

	if (maze[pacManRow][pacManCol] == COIN)
	{
		coins += pacCell->GetCoin_value();
	}

	//update main maze
	maze[oldPac->GetRow()][oldPac->GetCol()] = SPACE;
	maze[pacManRow][pacManCol] = PACMAN;




	//face direction of pa-kman
	double width = 2.0 / MSZ;
	glPushMatrix();

	if (pacManCol < oldPac->GetCol())
	{
		faceLeft = true;
	}

	if (pacManCol > oldPac->GetCol())
	{
		faceLeft = false;
	}

	if (faceLeft)
	{
		glTranslated(-2 * (MSZ / 2.0 - 0.5 - pacManCol) * width, 0, 0);
		glScaled(-1, 1, 1);
	}

	DrawPacMan();

	glPopMatrix();



}

void PacMan::DrawPacMan()
{

	double  theta;
	double width = 2.0 / MSZ;
	double x = 2 * (pacCell->GetCol() / (double)MSZ) - 1;
	double y = 2 * (pacCell->GetRow() / (double)MSZ) - 1;

	double 	r = width * 0.33;
	double 	num_segments = 15;
	double 	cx = x + width / 2;
	double 	cy = y + width / 2;

	glColor3d(1, 1, 0);

	glBegin(GL_POLYGON);
	for (int ii = 0; ii < num_segments; ii++)
	{
		theta = 2.0 * 3.1415926 * double(ii) / double(num_segments);//get the current angle 
		x = r * cos(theta);//calculate the x component 
		y = r * sin(theta);//calculate the y component 
		glVertex2d(x + cx, y + cy);//output vertex 
	}
	glEnd();

	glColor3d(0, 0, 0);

	glBegin(GL_POLYGON);
	for (int ii = 0; ii < num_segments; ii++)
	{
		theta = 2.0 * 3.1415926 * double(ii) / double(num_segments);//get the current angle 
		x = 0.25 * r * cos(theta);//calculate the x component 
		y = 0.25 * r * sin(theta);//calculate the y component 
		glVertex2d(x + cx - 0.005, y + cy + 0.01);//output vertex 
	}
	glEnd();

	glBegin(GL_LINE_STRIP);

	theta = 2.0 * PI * double(1) / double(num_segments);
	x = r * cos(theta);//calculate the x component 
	y = r * sin(theta);//calculate the y component 
	glVertex2d(x + cx, y + cy);//output vertex 

	glVertex2d(cx, cy);

	theta = 2.0 * PI * double(-1) / double(num_segments);//get the current angle 
	x = r * cos(theta);//calculate the x component 
	y = r * sin(theta);//calculate the y component 
	glVertex2d(x + cx, y + cy);//output vertex 


	for (int ii = -1; ii < num_segments / 4 - 1; ii++)
	{
		theta = 2.0 * 3.1415926 * ii / num_segments;//get the current angle
		x = r * cos(theta);//calculate the x component
		y = r * sin(theta);//calculate the y component
		glVertex2d(x + cx, y + cy);//output vertex
	}

	glEnd();


}