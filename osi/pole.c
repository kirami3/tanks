#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <ncurses.h>

int main()
{
	int h;
	int *gi;
	int *gj;
	initscr();

	scanf("%d", &h);
	gi = (int*)malloc(4 * h);
        gj = (int*)malloc(4 * h);
        srand(time(NULL));
        for (int m; m < h; m++)
        {
                gi[m] = rand() % 28;
                gj[m] = rand() % 78;
		while ((gj[m] == 36 || gj[m] == 37 || gj[m] == 38 || gj[m] == 39) && gi[m] == 14)
			gj[m] = rand() % 78;
                mvaddch(gi[m], gj[m], '#');
        }
	getch();
	refresh();
	endwin();
        return (0);
}

