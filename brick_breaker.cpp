#include <iostream>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <unistd.h>
#include <algorithm>
using namespace std;

//used to move the cursor to a specific position on the screen
void gotoxy(int x, int y)
{
	COORD c; //  a structure having two members which are the X and Y coordinates
	c.X = x; // Locating its x coordinate
	c.Y = y; // Locating its y coordinate
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c); // Set the position for next thing to be printed
}

//used to hide the cursor
void hideCursor()
{
	CONSOLE_CURSOR_INFO cursor; // Create a CONSOLE_CURSOR_INFO object
	cursor.dwSize = 100; // Assigning size of cursor
	cursor.bVisible = false; // Making cursor invisible
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor); // Set the cursor for next thing to be printed
}


//Some variables used in the game
int life;
int const screenHeight = 20;
int const screenWidth = 30;
int map[screenHeight][screenWidth];
char bricks[8][18] = {"##..##.##.######.",
					  "##..##.##...##...",
					  "##..##.##...##...",
					  "######.##...##...",
					  "##..##.##...##...",
					  "##..##.##...##...",
					  "##..##.##...##..."};

char temp[8][18] = {"##..##.##.######.",
                    "##..##.##...##...",
                    "##..##.##...##...",
                    "######.##...##...",
                    "##..##.##...##...",
                    "##..##.##...##...",
                    "##..##.##...##..."};

bool decre_life; //used to decrease the life of the player when the ball hits the bottom of the screen

class Paddle
{
	public:
		int x;
		int y;
		float speed;
		char dir;
		int delay;
		int count_delay;

	void draw()
	{
		for(int i = 0; i < 9; i++)
			map[y][x+i] = 1; // 1 is the ASCII code the character to make the paddle
	}

	void move()
	{
		if(count_delay == delay)
		{ //used to make the paddle move slower
			if(dir == 'L' && x-speed > 0)
            {  // used to make the paddle move left
				x -= speed;
			}
			else if(dir == 'R' && x+speed < screenWidth-9)
			{ //used to make the paddle move right
				x += speed;
			}
			count_delay = 0;
		}
		count_delay++;
		if(decre_life)
        {
			dir = 'S';
			x = screenWidth/2-5;
			y = screenHeight - screenHeight/7-1;
			decre_life = false;
		}
	}
};

class Ball
{
	public:
		int x;
		int y;
		int speed;
		int dir;

	void draw()
	{
		map[y][x] = 5;
	}

	void move()
	{
	    sleep(0.1);//unsigned int
	    Sleep(00000001.0);//doubleword Dw milliseconds
		if(dir == 0 && !collision(x-speed, y-speed))
        { //used to make the ball move in the top left direction
			x -= speed;
			y -= speed;
		}
		else if(dir == 1 && !collision(x+speed, y-speed))
        { //top right direction
			x += speed;
			y -= speed;
		}
		else if(dir == 2 && !collision(x-speed, y+speed))
        { //bottom left direction
			x -= speed;
			y += speed;
		}
		else if(dir == 3 && !collision(x+speed, y+speed))
		{ //bottom right direction
			x += speed;
			y += speed;
		}
	}

	//used to check if the ball collides with the paddle or the bricks
	bool collision(int fx, int fy)
	{
	    	    sleep(0.1);
		if(map[fy][x] == 8)//used to check if the ball collides with the paddle
        {
			decre_life = true;
			x = screenWidth/2-1;
			y = screenHeight - screenHeight/7-3;
			dir = 4;
			life--;
		}
		//used to check if the ball collides with the bricks
		else if(map[fy][fx] != 0 || map[y][fx] != 0 || map[fy][x] != 0 || map[fy][fx] == 2 || map[y][fx] == 2 || map[fy][x] == 2)
        {
			if(map[fy][fx] == 2) bricks[fy-2][fx-6] = '.';
			if(map[y][fx] == 2) bricks[y-2][fx-6] = '.';
			if(map[fy][x] == 2) bricks[fy-2][x-6] = '.';

			if(map[y][fx] != 0) bounce(fx,y);
			else if(map[fy][x] != 0) bounce(x,fy);
			else if(map[fy][fx] != 0) bounce(fx,fy);

			return true;
		}

		return false;
	}

	//used to change the direction of the ball when it collides with the paddle or wall
	void bounce(int fx, int fy)
	{
	       sleep(0.1);//unsigned int
	       Sleep(00000001.0);//doubleword DW millieseconds
		if(dir == 0)
        {
			if(fx < x) dir = 1;
			else if(fy < y) dir = 2;
			else if(fx < x && fy < y) dir = 0;
		}
		else if(dir == 1)
		{
			if(fx > x) dir = 0;
			else if(fy < y) dir = 3;
			else if(fx > x && fy < y) dir = 1;
		}
		else if(dir == 2)
		{
			if(fx < x) dir = 3;
			else if(fy > y) dir = 0;
			else if(fx < x && fy > y) dir = 2;
		}
		else if(dir == 3)
		{
			if(fx > x) dir = 2;
			else if(fy > y) dir = 1;
			else if(fx > x && fy > y) dir = 3;
		}
	}
};

//used to draw the bricks
void brick()
{
	for(int i = 0; i < 7; i++)
    {
		for(int j = 0; j < 17; j++)
		{
			if(bricks[i][j] == '#') map[i+2][j+6] = 2; // 2 is the ASCII code the character to make the bricks
		}
	}
}

Paddle paddle;
Ball ball;

void setup()
{
	srand(time(NULL));
	decre_life = false;
	life = 5;
	paddle.x = screenWidth/2-5;
	paddle.y = screenHeight - screenHeight/7-1;
	paddle.speed = 1.0;//0.01
	paddle.delay = 1;
	ball.x = screenWidth/2;
	ball.y = screenHeight - screenHeight/7-2;
	ball.speed = 1;
	ball.dir = rand()%4;
}

//used to make the wall
void wall()
{
	for(int i = 0; i < screenHeight; i++)
    {
		for(int j = 0; j < screenWidth; j++)
        {
			if(j == 0 || j == screenWidth-1) map[i][j] = 9; // 9 is the ASCII code the character to make the wall
			else if(i == 0) map[i][j] = 7; // 7 is the ASCII code the character to make the top wall
			else if(i == screenHeight-1) map[i][j] = 8; // 8 is the ASCII code the character to make the bottom wall
			else map[i][j] = 0; // 0 is the ASCII code the character to make the background playable
		}
	}
}

void layout()
{
	wall();
	paddle.draw();
	ball.draw();
	brick();
}

void display()
{
	gotoxy(2,1); cout << "            LIFE: " << life;

	for(int i = 0; i < screenHeight; i++)
	{
		for(int j = 0; j < screenWidth; j++)
        {
			gotoxy(j+2, i+3);
			if(map[i][j] == 9) cout << char(219);
			if(map[i][j] == 1) cout << char(219);
			if(map[i][j] == 2) cout << char(233);
			if(map[i][j] == 7) cout << char(219);
			if(map[i][j] == 8) cout << char(240);
			if(map[i][j] == 5) cout << char(254);
			if(map[i][j] == 0) cout << char(32);
		}
	}
}

void input()
{
	if(_kbhit())
    {
		switch(getch())
		{
			case 75:
				paddle.dir = 'L';
				break;
			case 77:
				paddle.dir = 'R';
				break;
		}
		if(ball.dir == 4) ball.dir = rand()%2;
	}
}

void movements()
{
	paddle.move();
	ball.move();
}

void gameOver()
{
	system("cls");
	char text[20]={" G A M E O V E R "};
	int j;
	int z=strlen(text);
	cout<<"\n\n\t\t";
	for(j=0;j<=25;j++){
		Sleep(50);
		cout<<"\xDB";
	}
	for(j=0;j<=z;j++){
		Sleep(60);
		cout<<text[j];
	}
	for(j=0;j<=25;j++){
		Sleep(50);
		cout<<"\xDB";
	}
	cout<<"\n\n\t\t";
	cout<<"\xB3\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 Do you want to play again?y/n\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB3";
    cout<<"\n\n\t\t";
}
void introduction()
{
system("color C0");

cout<<R"(











888888b.   8888888b.  8888888 .d8888b.  888    d8P       888888b.   8888888b.  8888888888        d8888 888    d8P  8888888888 8888888b.
888  "88b  888   Y88b   888  d88P  Y88b 888   d8P        888  "88b  888   Y88b 888              d88888 888   d8P   888        888   Y88b
888  .88P  888    888   888  888    888 888  d8P         888  .88P  888    888 888             d88P888 888  d8P    888        888    888
8888888K.  888   d88P   888  888        888d88K          8888888K.  888   d88P 8888888        d88P 888 888d88K     8888888    888   d88P
888  "Y88b 8888888P"    888  888        8888888b         888  "Y88b 8888888P"  888           d88P  888 8888888b    888        8888888P"
888    888 888 T88b     888  888    888 888  Y88b        888    888 888 T88b   888          d88P   888 888  Y88b   888        888 T88b
888   d88P 888  T88b    888  Y88b  d88P 888   Y88b       888   d88P 888  T88b  888         d8888888888 888   Y88b  888        888  T88b
8888888P"  888   T88b 8888888 "Y8888P"  888    Y88b      8888888P"  888   T88b 8888888888 d88P     888 888    Y88b 8888888888 888   T88b






                                                                                                                                         )";
hideCursor();
sleep(6);
system("cls");
}

int main()
{
    system("color C0");
    system("title Brick Breaker Game");
    introduction();
	hideCursor();
	setup();
	while(true)
    {

        while(life > 0)
        {
		display();
		layout();
		input();
		movements();
        }
        char ch;
        gameOver();
        cin >> ch;
        if(ch == 'y' || ch == 'Y')
        {
            system("cls");
            for(int i=0; i<8; i++)
            {
                for(int j=0; j<18; j++)
                {
                    bricks[i][j] = temp[i][j];
                }
            }
            life = 5;
        }
        else
        {
            return 0;
        }
	}
}

