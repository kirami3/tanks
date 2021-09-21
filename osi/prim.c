#include <ncurses.h>

int main()
{
	int u;
	int y;
	char k;
	initscr();
	noecho();
	u = 0;
	k = 48;
	y = 0;
	keypad(stdscr, TRUE);
	for (int i = 0; i < LINES; i++)
	{
		mvaddch(u, y, k);
		refresh();
		u++;
		k++;
		y++;
	}
	mvaddch(3, COLS/2, '#');
	mvaddch(3, COLS - 1, '*');
	mvaddch(LINES - 1, 10, '*');
	getch();
	endwin();
	return (0);
}
