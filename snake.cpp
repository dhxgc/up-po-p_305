#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <ncurses.h>
#include <unistd.h>
#include <thread>
#include <stdio.h>
// #include <wiringPi.h>
using int32 = int;
using namespace std;

int Press_right_detected=0,Press_left_detected=0,Press_up_detected=0,Press_down_detected=0;
int left_btn=18, up_btn=19, right_btn=15, down_btn=16;

struct CharPosition {
	int x,y;
	CharPosition(int col, int row);
	CharPosition();
};

class fSnakeGame {
	private:
		int score, del, maxwidth, maxheight;
		char direction, snakesym, border, fruitchar;
		
		CharPosition fruit;
		std::vector<CharPosition> snake;
		
		void InitGameWindow();
		void DrawWindow();
		void DrawSnake();
		void PrintScore();
		void PositionFruit();
		void timer();
		
		bool FatalCollision();
		
		void MoveSnake();
		
		bool GetsFruit();

	public:
		bool bEatsFruit;
		int fruits;
		fSnakeGame();
		~fSnakeGame();
		void PlayGame();
};	



CharPosition::CharPosition(int col, int row) {
	x=col;
	y=row;
}

CharPosition::CharPosition() {
	x=0;
	y=0;
}

fSnakeGame::fSnakeGame() {
	snakesym='S';
	border='O';
	fruitchar='X';
	fruit.x=0;
	fruit.y=0;
	score=0;
	del=110000;
	direction='l';
	srand(time(NULL));
	
	InitGameWindow();
	PositionFruit();
	DrawWindow();
	DrawSnake();
	PrintScore();
	
	refresh();
}

fSnakeGame::~fSnakeGame() {
	nodelay(stdscr,false);
	getch();
	endwin();
}

// инициализация игрового окна (должна быть выполнена перед использованием ncurses)
void fSnakeGame::InitGameWindow() {
	initscr();
	nodelay(stdscr,TRUE);
	keypad(stdscr,true);
	noecho();
	curs_set(0);
	getmaxyx(stdscr,maxheight,maxwidth);
	return;
}

// Вставим символы в окно (рисуем игровое поле)
void fSnakeGame::DrawWindow() {
	for(int i=0;i<maxwidth;i++) {
		move(0,i);
		addch(border);
		}
	for(int i=0;i<maxwidth;i++) {
		move(maxheight-2,i);
		addch(border);
		}
	for(int i=0;i<maxheight-1;i++) {
		move(i,0);
		addch(border);
		}
	for(int i=0;i<maxheight-1;i++) {
		move(i,maxwidth-1);
		addch(border);
		}
	return;
}

// Рисуем тело змейки
void fSnakeGame::DrawSnake() {
	for(int i=0;i<5;i++) {
		snake.push_back(CharPosition(30+i,10));
		}
	for(int i=0;i<snake.size();i++) {
		move(snake[i].y,snake[i].x);
		addch(snakesym);
		}
	return;
}

// Выводим текущий счет на экран
void fSnakeGame::PrintScore() {
	move(maxheight-1,0);
	printw("Points: %d", score);
	return;
}

// Рандомно вставляем фрукт на игровое поле
void fSnakeGame::PositionFruit() {
	while(1) {
		int tmpx=rand()%maxwidth+1;
		int tmpy=rand()%maxheight+1;
		
		for(int i=0;i<snake.size();i++) {
			if(snake[i].x==tmpx && snake[i].y==tmpy) {
				continue;
				}
			}
		
		if(tmpx>=maxwidth-2 || tmpy >=maxheight-3) {
			continue;
			}
		fruit.x=tmpx;
		fruit.y=tmpy;
		break;
	}
	
	move(fruit.y,fruit.x);
	addch(fruitchar);
	refresh();
}

// Обработка столкновений и конца игры
bool fSnakeGame::FatalCollision() {
	if(snake[0].x==0 || snake[0].x==maxwidth-1 || snake[0].y==0 || snake[0].y==maxheight-2) {
		return true;
		}
		
	for(int i=2;i<snake.size();i++) {
		if(snake[0].x==snake[i].x && snake[0].y==snake[i].y) {
			return true;
			}
		}
	return false;
}

// Обработка съедания фрукта
bool fSnakeGame::GetsFruit() {
	if(snake[0].x==fruit.x && snake[0].y==fruit.y) {
		PositionFruit();
		score+=10;
		PrintScore();
		
		if((score%100)==0) {
			del-=1000;
			}
		return bEatsFruit=true;
		} else {
			return bEatsFruit=false;
			}
	return bEatsFruit;
}

// Направление движения змеи на клавиатуре и кнопках на стенде
void fSnakeGame::MoveSnake() {
	if(Press_left_detected==1) {
			if(direction!='r') {
				direction='l';
				}
			Press_left_detected=0;
		}

	if(Press_right_detected==1) {
			if(direction!='l') {
				direction='r';
				}
				Press_right_detected=0;
		}
		
	if(Press_down_detected==1) {
			if(direction!='u') {
				direction='d';
				}
				Press_down_detected=0;
		}
	if(Press_up_detected==1) {
			if(direction!='d') {
				direction='u';
				}
				Press_up_detected=0;
		}
	
	int KeyPressed=getch();
	switch(KeyPressed) {
		case KEY_LEFT:
			if(direction!='r') {
				direction='l';
				break;
				}
		case KEY_RIGHT:
			if(direction!='l') {
				direction='r';
				break;
				}
		case KEY_UP:
			if(direction!='d') {
				direction='u';
				break;
				}
		case KEY_DOWN:
			if(direction!='u') {
				direction='d';
				break;
				}
		case KEY_BACKSPACE:
			direction='q';
			break;
		}
	if(!bEatsFruit) {
		move(snake[snake.size()-1].y,snake[snake.size()-1].x);
		printw(" ");
		refresh();
		snake.pop_back();
		}
		
	if(direction=='l') {
		snake.insert(snake.begin(), CharPosition(snake[0].x-1, snake[0].y));
	} 
	else if (direction=='r') {
		snake.insert(snake.begin(), CharPosition(snake[0].x+1, snake[0].y));
		} 
	else if (direction=='u') {
		snake.insert(snake.begin(), CharPosition(snake[0].x, snake[0].y-1));
		} 
	else if (direction=='d') {
		snake.insert(snake.begin(), CharPosition(snake[0].x, snake[0].y+1));
			}		
	move(snake[0].y,snake[0].x);
	addch(snakesym);
	refresh();
	return;
}

void fSnakeGame::PlayGame() {
	while(1) {
		if(FatalCollision()) {
			move((maxheight-2)/2,(maxwidth-5)/2);
			printw("GAME OVER");
			break;
			}
		GetsFruit();
		MoveSnake();
		
		if(direction=='q') {
			break;
			}
		usleep(del);
		}
}

int maxheight, maxwidth;
void PlayGame();
int IsUserReady();
int AskUserToPlayAgain();
void ClearCentre();
int UserInput();

void Press_up_Interrupt(void) {
	Press_up_detected=1;
	delay(100);
}
void Press_down_Interrupt(void) {
	Press_down_detected=1;
	delay(100);
}
void Press_left_Interrupt(void) {
	Press_left_detected=1;
	delay(100);
}
void Press_right_Interrupt(void) {
	Press_right_detected=1;
	delay(100);
}

int main() {	
	wiringPiSetupPhys();
	pinMode(up_btn, INPUT);
	pinMode(down_btn, INPUT);
	pinMode(left_btn, INPUT);
	pinMode(right_btn, INPUT);	
	wiringPiISR(up_btn,INT_EDGE_BOTH, &Press_up_Interrupt);
	wiringPiISR(down_btn,INT_EDGE_BOTH, &Press_down_Interrupt);
	wiringPiISR(left_btn,INT_EDGE_BOTH, &Press_left_Interrupt);
	wiringPiISR(right_btn,INT_EDGE_BOTH, &Press_right_Interrupt);
	if(IsUserReady()=='y')
	do {
		{
			fSnakeGame NewSnake;
			NewSnake.PlayGame();
			}
		}
	while(AskUserToPlayAgain()=='y');
	return 0;
}

void ClearCentre(float x, float y) {
	clear();
	initscr();
	noecho();
	getmaxyx(stdscr, maxheight, maxwidth);
	move((maxheight/y), (maxwidth/x));
}

int UserInput() {
	int UserInput=getch();
	refresh();
	endwin();
	clear();
	
	return UserInput;
}

int IsUserReady() {
	ClearCentre(2.5, 2.5);
	printw("Press y to start (y/n)");
	return UserInput();
}

int AskUserToPlayAgain() {
	ClearCentre(2.5, 2.5);
	printw("Press y to play again (y/n)");
	return UserInput();
	}