//#include "pch.h"
#include <iostream>
#include <conio.h>
//#include <windows.h>
#include <stdlib.h>
using namespace std;

int p1Score, p2Score;
const int width = 21;
const int height = 21;
const int goalLeft = (width / 2) - (width / 8);
const int goalRight = (width / 2) + (width / 8);
const int winningScore = 3;

bool quit;

enum direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
direction p1Dir;
direction p2Dir;

class Puck {
public:
	//position of puck
	int x, y;
	//velocity of puck
	int dx, dy;
};

class Player {
public:
	//position of player
	int x, y;
};

void drawRink(Puck* p, Player* p1, Player* p2) {
	//clear screen
	system("cls");
	cout << endl;
	//print score
	cout << "P1: " << p1Score << "  P2: " << p2Score << endl;

	//draw top border and goal
	for (int i = 0; i < width; i++) {
		if (i < goalLeft || i > goalRight) {
			cout << "*";
		}
		else {
			cout << " ";
		}
	}
	cout << endl;

	//draw puck, players, side borders, midline
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (j == (*p).x && i == (*p).y) {
				cout << "o";      //puck
			}
			else if ((j == (*p1).x && i == (*p1).y) || (j == (*p2).x && i == (*p2).y)) {
				cout << "0";                    //players
			}
			else if (j == 0 || j == width - 1) {
				cout << "#";                    //borders
			}
			else if (i == width / 2) {
				cout << "-";				//midline
			}
			else {
				cout << " ";      //empty space
			}
		}
		cout << endl;
	}

	//draw bottom border and goal
	for (int i = 0; i < width; i++) {
		if (i < goalLeft || i > goalRight) {
			cout << "#";
		}
		else {
			cout << " ";
		}
	}
	cout << (*p).x << (*p).y << endl;
	cout << endl;
}

void initialize(Puck* p, Player* p1, Player* p2) {
	//initialize puck position
	(*p).x = width / 2;
	(*p).y = height / 2;

	//initialize random puck velocity
	//random int between 0 and 1
	int randX = rand() % 2; 
	int randY = rand() % 2;
	if(randX == 0){
		(*p).dx = -1;
	}else{
		(*p).dx = 1;
	}
	if(randY == 0){
		(*p).dy = -1;
	}else{
		(*p).dy = 1;
	}
	
	//initialize p1
	(*p1).x = width / 2;
	(*p1).y = 3 * (height / 4);

	//initialize p2
	(*p2).x = width / 2;
	(*p2).y = height / 4;
}

void setup(Puck* p, Player* p1, Player* p2) {
	quit = false;
	p1Score = 0;
	p2Score = 0;
	p1Dir = STOP;
	p2Dir = STOP;
	initialize(p, p1, p2);
	drawRink(p, p1, p2);
}


void input(){
/*  w   //p1 controls
  a   d
    s

    i     //p2 controls
  j   l
    k      */

	if(_kbhit()){
		switch(_getch()){
		//quit
		case 'q':
			quit = true;
			break;
		//p1 buttons
		case 'a':
			p1Dir = LEFT;
			break;
		case 'd':
			p1Dir = RIGHT;
			break;
		case 'w':
			p1Dir = UP;
			break;
		case 's':
			p1Dir = DOWN;
			break;
		//p2 buttons
		case 'j':
			p2Dir = LEFT;
			break;
		case 'l':
			p2Dir = RIGHT;
			break;
		case 'i':
			p2Dir = UP;
			break;
		case 'k':
			p2Dir = DOWN;
			break;
		}
	}
}

bool score(Puck* p, Player* p1, Player* p2) {
//checks if puck is in the goal
	int xCoor = (*p).x;
	int yCoor = (*p).y;
	if (xCoor < goalLeft || xCoor > goalRight) {
		return false;
	}else {
		if (yCoor >= height) {
			p2Score++;
			return true;
		}
		else if (yCoor <= -1) {
			p1Score++;
			return true;
		}
	}
	return false;
}

void changePlayerLocation(Player* p, direction* dir, string type) {
//moves player based on keyboard input
	switch (*dir) {
	case LEFT:
		if ((*p).x > 1) {
			(*p).x--;
		}
		break;
	case RIGHT:
		if ((*p).x < width-2) {
			(*p).x++;
		}
		break;
	case UP:
		if (type.compare("p1") == 0) {
			if ((*p).y > (height / 2) + 1) {
				(*p).y--;
			}
		}else {
			if ((*p).y > 0) {
				(*p).y--;
			}
		}
		break;
	case DOWN:
		if (type.compare("p1") == 0) {
			if ((*p).y < height - 1) {
				(*p).y++;
			}
		}else {
			if ((*p).y < (height/2) - 1) {
				(*p).y++;
			}
		}
		break;
	}
	*dir = STOP;
}

int dotProduct(Puck* p, int tangentX, int tangentY){
	return ((*p).dx * tangentX) + ((*p).dy * tangentY);
}

void addDirection(Puck* p, direction dir){
//adds the direction of the player to movement of puck
	switch(dir){
	case LEFT:
		(*p).dx--;
		break;
	case RIGHT:
		(*p).dx++;
		break;
	case UP:
		(*p).dy--;
		break;
	case DOWN:
		(*p).dy++;
		break;
	}
}

void perpendicularVect(Puck* p, int tangentX, int tangentY){
//gives reflection of puck velocity vector

	//top or bottom of player
	if(tangentY == 0){
		(*p).dy *= -1;

	//sides of player
	}else if(tangentX == 0){
		(*p).dx *= -1;

	//diagonal tangents of player
	}else{

		//positive or negative direction based on tangent vector (or position on the octagon)
		int position = tangentX * tangentY;
		int temp = (*p).dy * position;
		(*p).dy = (*p).dx * position;
		(*p).dx = temp;
	}
}

void puckHitsPlayer(Puck* p, int pXDiff, int pYDiff, direction dir){
	//Player is represented as an octagon (tangent lines to surface of circle at 0, pi/4, p/2, 3pi/4, pi, etc.)

	//tangent line to player 
	//flip x and y and make x negative
	//makes dot product 0 (thus perpendicular) and consistent clockwise direction
	int temp = pXDiff;
	int tangentX = -1*pYDiff;
	int tangentY = temp;

	//vector from puck to player perpendicular to tangent lines of player
	if(dotProduct(p, tangentX, tangentY) == 0){
		(*p).dx *= -1;
		(*p).dy *= -1;
		addDirection(p, dir);

	//vector from puck to player not perpendicular to tangent lines of player
	}else{
		perpendicularVect(p, tangentX, tangentY);
		addDirection(p, dir);
	}

	//if puck and player share the same location, move the puck
	if(pXDiff == 0 && pYDiff == 0){
		(*p).x += (*p).dx;
		(*p).y += (*p).dy;
	}
}

void mechanics(Puck* p, Player* p1, Player* p2) {
	int puckX = (*p).x;
	int puckY = (*p).y;
	int play1X = (*p1).x;
	int play1Y = (*p1).y;
	int play2X = (*p2).x;
	int play2Y = (*p2).y;

	//vector from player to puck
	int p1XDiff = puckX - play1X;
	int p1YDiff = puckY - play1Y;
	int p2XDiff = puckX - play2X;
	int p2YDiff = puckY - play2Y;

	//puck hits player
	if((p1XDiff <= 1 &&  p1XDiff >= -1) && (p1YDiff <= 1 && p1YDiff >= -1)){
		puckHitsPlayer(p, p1XDiff, p1YDiff, p1Dir);
	}else if((p2XDiff <= 1 && p2XDiff >= -1) && (p2YDiff <= 1 && p2YDiff >= -1)){
		puckHitsPlayer(p, p2XDiff, p2YDiff, p2Dir);
	}

	//change location of p1
	changePlayerLocation(p1, &p1Dir, "p1");
	
	//change location of p2
	changePlayerLocation(p2, &p2Dir, "p2");

	//change puck location
	(*p).x += (*p).dx;
	(*p).y += (*p).dy;

	//puck goes in goal
	if (score(p, p1, p2)) {
		initialize(p, p1, p2);
		return;
	}
	//puck hits side walls
	if ((*p).x >= width-1){
		(*p).dx *= -1;
		(*p).x = width - 2;
	}else if((*p).x <= 0) {
		(*p).dx *= -1;
		(*p).x = 1;
		
	//puck hits top or bottom wall
	}else if ((*p).y >= height){
		(*p).dy *= -1;
		(*p).y = height - 1;
	}else if((*p).y <= -1) {
		(*p).dy *= -1;
		(*p).y = 0;
	}
}

int main() {
	Puck p;
	Player p1;
	Player p2;
	setup(&p, &p1, &p2);
	while (p1Score < winningScore && p2Score < winningScore) {
		input();
		if(quit){
			break;
		}
		mechanics(&p, &p1, &p2);
		drawRink(&p, &p1, &p2);
		//Sleep(5);
	}
	if (p1Score == p2Score) {
		cout << "Tie.";
	}
	else if (p1Score > p2Score) {
		cout << "Player 1 Wins!!!!!";
	}
	else {
		cout << "Player 2 Wins!!!!!";
	}
	return 0;
}
