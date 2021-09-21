#include <unistd.h>
#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


int	main()
{
	int i;
	int l;
	int si;
	int sj;
	int j;
	int t;
	int h;
	int chet;
	int *gi;
	int *gj;
	char k;
	char v;
	int ch;
	initscr();
start:
	i = LINES/2;
	j = COLS/2;
	keypad(stdscr, TRUE);
	curs_set(0);
	v = 'F';
	while (1)
	{
		ch = getch();
		switch (ch)
		{
		case KEY_DOWN:
			k = '0';
			t = 1;
			break;
		case KEY_RIGHT:
			k = '3';
			t = 1;
			break;
		case KEY_LEFT:
			k = '2';
			t = 1;
			break;
		case KEY_UP:
			k = '1';
			t = 1;
			break;
		default:
			break;
		}
		switch (k)
		{
		case '0':
			if (i == LINES)
				i = 0;
			else
				i++;
			v = 'v';
			break;
		case '1':
			if (i == 0)
				i = LINES;
			else
				i--;
			v = '^';
			break;
		case '2':
			if (j == 0)
				j = COLS;
			else
				j--;
			v = '<';
			break;
		case '3':
			l = 8;
			if (j == COLS)
				j = 0;
			else
				j++;
			v = '>';
			break;
		default:
			break;
		}
		mvaddch(i, j, v);
		refresh();
		usleep(500000);
	}
	
	clear();
	timeout(-1);
	mvaddstr(LINES/2, COLS/2, "Congratulations!\n\t If you want to play again, press ENTER");
	k = getch();
	if (k == '\n')
		goto start;
	endwin();
	return (0);
}

