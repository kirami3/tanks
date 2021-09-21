#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

# define SIMB 'F'

void print(int x, int y)
{
	mvaddch(x, y, SIMB);
}

int main()
{
	int x;
	int y;
	int p;
	int ch;

	initscr();
	keypad(stdscr, TRUE);
	noecho();
	timeout(1);
	x = LINES/2;
	y = COLS/2;
	mvaddch(x, y, 'F');
	while((ch = getch()) != 'f')
	{
		mvaddch(x, y, ' ');
		if (ch == 'q')
		{
			p = 1;
		}
		if (ch == 'w')
			p = 2;
		if (ch == 'e')
		{
			p = 3;
		}
		if (ch == 'd')
			p = 4;
		if (ch == 'c')
		{
			p = 5;
		}
		if (ch == 'x')
			p = 6;
		if (ch == 'z')
		{
			p = 7;
		}
		if (ch == 'a')
			p = 8;
		if (p == 1)
		{
			x--;
			y--;
			if (x == 0 || y == 0)
				p = 5;

		}
		else if (p ==2)
		{
			x--;
			if (x == 0)
				p = 6;
		}
		else if (p==3)
		{
			x--;
			y++;
			if (x == 0 || y == COLS)
				p = 7;
		}
		else if (p==4)
		{
			y++;
			if (y == COLS)
				p = 8;
		}
		else if (p==5)
		{
			x++;
			y++;
			if (x == LINES || y == COLS)
				p = 1;
		}
		else if (p==6)
		{
			x++;
			if (x == LINES)
				p = 2;
		}
		else if (p==7)
		{
			x++;
			y--;
			if (x== LINES || y == 0)
				p = 3;
		}
		else if (p==8)
		{
			y--;
			if (y == 0)
				p = 4;
		}


		print(x, y);
		refresh();
		usleep(50000);
	//	mvaddch(x, y, 'F');
	}
	endwin();
	return (0);
}




