/*
**************************
** Made By Tamir Reznik **
**************************
*/


#include "glut.h"
#include <time.h>
#include <vector>
#include <stack>
#include <iostream>
#include "main.h"
#include "Monster.h"
#include "CompareCells.h"

using namespace std;

int maze[MSZ][MSZ] = { 0 }; // 0 is SPACE

bool pacman_is_on, monsters_alive;

Monster* monsterRed, * monsterGreen, * monsterBlue;
PacMan* pacMan;
int pacManRow, pacManCol;

int stepCounter, rateOfSearch;

void InitMaze()
{
	int i, j;

	// set margins
	for (i = 0; i < MSZ; i++)
	{
		maze[0][i] = WALL;
		maze[MSZ - 1][i] = WALL;
		maze[i][0] = WALL;
		maze[i][MSZ - 1] = WALL;
	}

	for (i = 1; i < MSZ - 1; i++)
	{

		for (j = 1; j < MSZ - 1; j++)
		{
			if (i % 2 == 1)  // this is mostly SPACE
			{
				if (rand() % 100 < 10) // 10% WALLs
					maze[i][j] = WALL;
				else
					maze[i][j] = COIN;
			}
			else // this is mostly WALL
			{
				if (rand() % 100 < 65) // 65% SPACEs
					maze[i][j] = COIN;
				else
					maze[i][j] = WALL;
			}
		}
	}

}

void init_Monsters()
{

	Cell::num_of_Monsters = 0;

	//red monster
	monsterRed = new Monster(new Cell(MSZ - 4, MSZ - 4, nullptr), 1, 0, 0, maze);


	//green monster
	monsterGreen = new Monster(new Cell(MSZ - 4, MSZ - 3, nullptr), 0, 1, 0, maze);


	//purple monster
	monsterBlue = new Monster(new Cell(MSZ - 4, MSZ - 5, nullptr), 0.93, 0.51, 0.93, maze);


	while (!(monsterRed->SearchForPac(maze) && monsterGreen->SearchForPac(maze) && monsterBlue->SearchForPac(maze)))
	{
		InitMaze();
		maze[pacManRow][pacManCol] = PACMAN;
		maze[monsterRed->GetCell()->GetRow()][monsterRed->GetCell()->GetCol()] = COIN_AND_MONSTER;
		maze[monsterGreen->GetCell()->GetRow()][monsterGreen->GetCell()->GetCol()] = COIN_AND_MONSTER;
		maze[monsterBlue->GetCell()->GetRow()][monsterBlue->GetCell()->GetCol()] = COIN_AND_MONSTER;
	}


	monsters_alive = true;

}

void init_PacMan_Game()
{

	Cell* pacCell = new Cell(4, 4, nullptr);
	pacManCol = pacCell->GetCol();
	pacManRow = pacCell->GetRow();
	maze[pacManRow][pacManCol] = PACMAN;

	init_Monsters();
	pacMan = new PacMan(pacCell);
	pacMan->RunAway(maze, true);// true for search is required in init

}

void init()
{
	glClearColor(0.8F, 0.7F, 0.5F, 0.0F);// color of window background
	glOrtho(-1, 1, -1, 1, 1, -1);

	srand((unsigned int)time(0));

	InitMaze();

	init_PacMan_Game();

	stepCounter = 1;
}

void DrawCoin(int row, int col)
{
	double  theta;
	double width = 2.0 / MSZ;
	double x = 2 * (col / (double)MSZ) - 1;
	double y = 2 * (row / (double)MSZ) - 1;

	double 	r = width * 0.4;
	double 	num_segments = 20;
	double 	cx = x + width / 2;
	double 	cy = y + width / 2;


	glColor3d(0, 0, 0);

	glBegin(GL_LINE_LOOP);
	for (int ii = 0; ii < num_segments; ii++)
	{
		theta = 2.0 * 3.1415926 * ii / num_segments;//get the current angle
		x = r * cos(theta);//calculate the x component
		y = r * sin(theta);//calculate the y component
		glVertex2d(x + cx, y + cy);//output vertex
	}
	glEnd();


	glColor3d(0.8, 0.84, 0.8);//silver
	glBegin(GL_POLYGON);
	for (int ii = 0; ii < num_segments; ii++)
	{
		glColor3d(0.8, 0.84, 0.8);
		//glColor3d(1, 0.84, 0);
		theta = 2.0 * 3.1415926 * ii / num_segments;//get the current angle
		x = r * cos(theta);//calculate the x component
		y = r * sin(theta);//calculate the y component
		glVertex2d(x + cx, y + cy);//output vertex


	}
	glEnd();



	glColor3d(1, 0.84, 0);//gold - inner part of coin
	glBegin(GL_POLYGON);
	for (int ii = 0; ii < num_segments; ii++)
	{

		theta = 2.0 * 3.1415926 * ii / num_segments;//get the current angle
		x = 0.6 * r * cos(theta);//calculate the x component
		y = 0.6 * r * sin(theta);//calculate the y component
		glVertex2d(x + cx, y + cy);//output vertex


	}
	glEnd();

}

void DrawMaze()
{
	int i, j;
	double sx, sy; // cell size
	double x, y;

	sx = 2.0 / MSZ;
	sy = 2.0 / MSZ;

	for (i = 0; i < MSZ; i++)
	{
		for (j = 0; j < MSZ; j++)
		{
			switch (maze[i][j])
			{
			case COIN_AND_MONSTER:
			case COIN:
			case PACMAN:
			case MONSTER:
			case SPACE:
				glColor3d(1, 1, 1);   // white
				break;
			case WALL:
				glColor3d(0.7, 0.4, 0);   // dark-orange
				break;
				/*case GRAY_START:
				case START:
					//glColor3d(0, 1, 0);   // cyan
					break;
				case TARGET:
					//glColor3d(1, 0, 0);   // red
					break;
				case GRAY:
					glColor3d(1, 1, 0);   // yellow
					break;
				case BLACK:
					glColor3d(0.2, 0.2, 0.2);   // black
					break;
				case PATH:
					glColor3d(0, 0.7, 1);   //  blue
					break;*/


			}
			// draw square maze[i][j]
			x = 2 * (j / (double)MSZ) - 1;
			y = 2 * (i / (double)MSZ) - 1;


			glBegin(GL_POLYGON);  // fill up
			glVertex2d(x, y);
			glVertex2d(x, y + sy);
			glVertex2d(x + sx, y + sy);
			glVertex2d(x + sx, y);
			glEnd();


			glColor3d(0, 0, 0);
			glBegin(GL_LINE_LOOP);  // black border
			glVertex2d(x, y);
			glVertex2d(x, y + sy);
			glVertex2d(x + sx, y + sy);
			glVertex2d(x + sx, y);
			glEnd();

			if (maze[i][j] == COIN || maze[i][j] == COIN_AND_MONSTER)
			{
				DrawCoin(i, j);
			}

		}

	}

}

bool IsPacDead()
{
	if (maze[monsterRed->GetCell()->GetRow()][monsterRed->GetCell()->GetCol()] == PACMAN
		|| maze[monsterGreen->GetCell()->GetRow()][monsterGreen->GetCell()->GetCol()] == PACMAN
		|| maze[monsterBlue->GetCell()->GetRow()][monsterBlue->GetCell()->GetCol()] == PACMAN || !pacman_is_on)
	{
		return true;
	}

	return false;
}

void DrawPacman()
{
	if (pacman_is_on)
	{
		Sleep(100);
		pacMan->PacManStep(maze, &pacman_is_on);
		monsterRed->MonsterStep(maze, &pacman_is_on);
		monsterGreen->MonsterStep(maze, &pacman_is_on);
		monsterBlue->MonsterStep(maze, &pacman_is_on);
		if (IsPacDead())
		{
			pacman_is_on = false;
			cout << "game over!\n score: " << pacMan->GetCoins() << "\n";
		}
		else
		{

			if (stepCounter % monsterBlue->GetsStepsBeforeSearch() == 0)
			{
				monsterBlue->SearchForPac(maze);
			}
			if (stepCounter % monsterRed->GetsStepsBeforeSearch() == 0)
			{
				monsterRed->SearchForPac(maze);
			}
			if (stepCounter % monsterGreen->GetsStepsBeforeSearch() == 0)
			{
				monsterGreen->SearchForPac(maze);
			}

		}

		stepCounter++;
	}

	if (!pacman_is_on && monsters_alive)
	{

		monsterRed->DrawMonster();

		monsterGreen->DrawMonster();

		monsterBlue->DrawMonster();

		pacMan->DrawPacMan();
	}

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer
	glLoadIdentity(); // start transformations fro identity matrix

	DrawMaze();

	DrawPacman();

	glutSwapBuffers(); // show all
}

void idle()
{
	glutPostRedisplay(); // indirect call to display
}

void menu(int choice)
{
	switch (choice)
	{

	case 1://PacMan
		pacman_is_on = true;
		break;
	case 2://New Maze

		pacman_is_on = false;
		monsters_alive = false;
		init();
		break;
	}
}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Pac-Man");

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	// menu
	glutCreateMenu(menu);
	glutAddMenuEntry("Start", 1);
	glutAddMenuEntry("New Game", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


	init();

	glutMainLoop();

	//return 0;

}