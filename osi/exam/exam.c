#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

# define SIMB 'F'

void print(int x, int y, char z)
{
	mvaddch(x, y, z);
}

int main()
{
	int x, y, p, ch, col;

	initscr();
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);
	init_pair(5, COLOR_WHITE, COLOR_BLACK);
	init_pair(6, COLOR_CYAN, COLOR_BLACK);
	init_pair(7, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(8, 14, COLOR_BLACK);
	keypad(stdscr, TRUE);
	noecho();
	curs_set(0);
	timeout(1);
	x = LINES/2;
	y = COLS/2;
	col = 0;
	print(x, y, SIMB);
	while ((ch = getch()) != 'f')
	{
//		print(x, y, ' ');
		switch (ch)
		{
		case 'q':
			p = 1;
			break;
		case 'w':
			p = 2;
			break;
		case 'e':
			p = 3;
			break;
		case 'd':
			p = 4;
			break;
		case 'c':
			p = 5;
			break;
		case 'x':
			p = 6;
			break;
		case 'z':
			p = 7;
			break;
		case 'a':
			p = 8;
			break;
		default:
			break;
		}
		switch (p)
		{
		case 1:
			if (x == 0 || y == 0)
			{
				p = 5;
				col = (col + 1) % 9;
				break;
			}
			x--;
			y--;
			break;
		case 2:
			if (x == 0)
			{
				p = 6;
				col = (col + 1) % 9;

				break;
			}
			x--;
			break;
		case 3:
			if (x == 0 || y == COLS - 1)
			{
				p = 7;
				col = (col + 1) % 9;
				break;
			}
			x--;
			y++;
			break;
		case 4:
			if (y == COLS - 1)
			{
				p = 8;
				col = (col + 1) % 9;
				break;
			}
			y++;
			break;
		case 5:
			if (x == LINES - 1 || y == COLS - 1)
			{
				col = (col + 1) % 9;
				p = 1;
				break;
			}
			x++;
			y++;
			break;
		case 6:
			if (x == LINES - 1)
			{
				p = 2;
				col = (col + 1) % 9;
				break;
			}
			x++;
			break;
		case 7:
			if (x == LINES - 1 || y == 0)
			{
				col = (col + 1) % 9;
				p = 3;
				break;
			}
			x++;
			y--;
			break;
		case 8:
			if (y == 0)
			{
				p = 4;
				col = (col + 1) % 9;
				break;
			}
			y--;
			break;
		default:
			break;
		}
		if (col == 0)
			col++;
		attron(COLOR_PAIR(col));
		print(x, y, SIMB);
		attroff(COLOR_PAIR(col));
		refresh();
		usleep(100000);
	}
	endwin();
	return (0);
}




