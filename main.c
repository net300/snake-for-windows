#include	<stdio.h>
#include	<stdlib.h> 
#include	<conio.h> 
#include	<time.h>
#include	<windows.h> 
#define 	maxofl 	 30
#define		maxx	 20
#define		maxy	 10
#define		speed 	 1
#define		up		 72
#define		down	 80
#define		left	 75
#define		right	 77
#define		wait_t   0.2
//You can change macros on this line to change the game settings.
#define 	getch()	_getch()
void ClearScreen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coordScreen = { 0, 0 };
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter(hConsole, ' ', dwConSize, coordScreen, &cCharsWritten);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
    SetConsoleCursorPosition(hConsole, coordScreen);
}
struct DIR{
	int xstep;
	int ystep;
}dir;
struct snake_t{
	int x[maxofl];
	int y[maxofl];
	int howlong;
	unsigned cos : 2;
}snake;
struct sweet_t{
	int x;
	int y;
}sweet;
void refsweet(){
ref:
	srand((unsigned int)time(NULL));
	sweet.x = rand() % maxx;
	sweet.y = rand() % maxy;
	if(sweet.x == snake.x[0] && sweet.y == snake.y[0])
	{
		goto ref;
	}
}
void movsnake(int xstep,int ystep){
	for (int i = 0; i < snake.howlong - 1;i++)
	{
		snake.x[snake.howlong - 1 - i] = snake.x[snake.howlong - 2 - i];
		snake.y[snake.howlong - 1 - i] = snake.y[snake.howlong - 2 - i];
	}
	if(!(snake.x[0] >= maxx || snake.y[0] >= maxy || snake.x[0] < 0 || snake.y[0] < 0))
	{
		snake.x[0] += xstep * speed;
		snake.y[0] += ystep * speed;
	}
	else{
		snake.x[0] = (snake.x[0] + maxx) % maxx;
		snake.y[0] = (snake.y[0] + maxy) % maxy;
		refsweet();
	}
}
void refresh(){
	int bl;
	int kbl; 
	ClearScreen();
	for(int j = 0;j < maxy; j++){
		for (int i = 0; i < maxx; i++)
		{
			bl = 0;
			kbl = 0;
			for (int k = 0;k < snake.howlong; k++){
				if(snake.x[k] == i && snake.y[k] == j)
				{
					if(k == 0){
						switch(snake.cos){
							case 0:
								printf("<");break;
							case 1:
								printf(">");break;
							case 2:
								printf("^");break;
							case 3:
								printf("v");break;
						}
					}
					else{
						printf("#");
					}
					bl = 1;
				}	
			}
			if(!bl && sweet.x == i && sweet.y == j){
				printf("@");
				kbl = 1;
			}
			if(!kbl && !bl){
				printf("-");
			}
		}
		printf("\n");
	} 
	printf("score:");
	printf("%d",snake.howlong);
}
void initsnake(){
	snake.howlong = 1;
	snake.x[0] = 0;
	snake.y[0] = 0;
}
void addsnake(){
	int _subx;
	int _suby;
	if(snake.howlong != maxofl)
	{
		snake.howlong+=1;
		switch(snake.cos)
		{
			case 0:
				_subx = -1;_suby = 0;break;
			case 1:
				_subx = 1;_suby = 0;break;
			case 2:
				_subx = 0;_suby = -1;break;
			case 3: 
				_subx = 0;_suby = 1;break;
		}
		snake.x[snake.howlong - 1] = (snake.x[snake.howlong - 2] - _subx);
		snake.y[snake.howlong - 1] = (snake.y[snake.howlong - 2] - _suby);
	}
}
int lsdied(){
	struct BL{
		unsigned one : 1;
	}bl = {0};
	for (int j = 1;j < snake.howlong; j++){
		if (bl.one == 0)
		{
			bl.one = (snake.y[0] == snake.y[j] && snake.x[0] == snake.x[j]);
		}
		else{
			break;
		}
	}
	return (bl.one); 
}
void movdir(int xstep,int ystep){
	dir.xstep = xstep;
	dir.ystep = ystep;
}
int main() {
	char ch;
	initsnake();
	refsweet();
	while(1){
		if(_kbhit())
		{
			ch = getch();
		}
		if(ch == 'q')
		{
			break;
		}
		switch(ch){
			case 'a':
			case left:
				movdir(-1,0);snake.cos = 0;break;
			case 'd':
			case right:
				movdir(1,0);snake.cos = 1;break;
			case 'w':
			case up:
				movdir(0,-1);snake.cos = 2;break;
			case 's':
			case down:
				movdir(0,1);snake.cos = 3;break;			
		}
		refresh();
		movsnake(dir.xstep,dir.ystep);
		Sleep(wait_t * 1000);
		if(snake.x[0] == sweet.x && snake.y[0] == sweet.y){
			addsnake();
			refsweet();
		}
		if(lsdied())
		{
			system("cls");
			printf("GAME OVER");
			break;
		}
		ch = 0;
	}
	return 0;
}
