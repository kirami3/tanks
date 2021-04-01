#include <unistd.h>
#include <stdio.h>
#include <ncurses.h>

int	main(int argc, char **argv)
{
	int i;
	int nul;
	int b;
	int j;
	int chet;
	char x;
	unsigned ch;
	int stroka[4] = {0};
	int a[4] = {0};
	int r[4][4] = {0};
	int c[4][4] = {0};
	int t[4] = {0};
	int l = 9;

	initscr();
	keypad(stdscr, TRUE);
	noecho();
	halfdelay(100);

	mvaddstr(LINES/2, COLS/2, "Enter values or 'q' if matrix is complete");
	mvaddstr(1, 1, "t[4] = {");
	for (i = 0; i < 4; i++)
        {
                mvaddch(1, l, t[i] + 48);
                if (i < 3)
                {
                        mvaddstr(1, l = l + 1, ", ");
                        l++;
                }
                else
                        mvaddch(1, l = l + 1, '}');
                refresh();
                l++;
        }
	mvaddstr(3, 1, "c[4][4] = ");
        for (i = 0; i < 4; i++)
        {
                for (j = 0; j < 4; j++)
                {
                        mvaddch(i + 3, 11 + 2 * j, c[i][j] + 48);
                        mvaddch(i + 3, 12 + 2 * j, ' ');
                }
        }
	mvaddstr(3, 22, "r[4][4] = ");
        for (i = 0; i < 4; i++)
        {
                for (j = 0; j < 4; j++)
                {
                        mvaddch(i + 3, 32 + j * 2, r[i][j] + 48);
                        mvaddch(i + 3, 33 + j * 2, ' ');
                }
        }
	mvaddstr(1, 22, "a[4] = {0, 0, 0, 0}");
        refresh();   
	
	i = 1;
	j = 9;
	move(i, j);
	while ((ch = getch()) != 'q')
	{
		switch (ch)
		{
		case KEY_RIGHT:
			if (j < 18)
				j += 3;
			break;
		case KEY_LEFT:
			if (j > 9)
				j -= 3;
			break;
		default:
			break;
		}
		move(i, j);
		if (ch >= '0' && ch <= '9')
		{
			t[(j - 9) / 3] = ch - 48;
			mvaddch(i, j, t[(j - 9) / 3] + 48);
		}
		refresh();
	}
	i = 3;
	j = 11;
	move(i, j);
	while ((ch = getch()) != 'q')
	{
		switch (ch)
		{
		case KEY_UP:
			if (i > 3)
				i--;
			break;
		case KEY_RIGHT:
			if (j < 17)
				j += 2;
			break;
		case KEY_LEFT:
			if (j > 11)
				j -= 2;
			break;
		case KEY_DOWN:
			if (i < 6)
				i++;
			break;
		default:
			break;
		}
		move(i, j);
		if (ch >= '0' && ch <= '9' && ch <= t[(j - 11) / 2] + 48)
		{
			c[i - 3][(j - 11) / 2 ] = ch - 48;
			mvaddch(i, j, c[i - 3][(j - 11) / 2] + 48);
		}
		refresh();
	}
	mvaddstr(LINES/2, COLS/2, "                                               ");
	l = 30;
        for (j = 0; j < 4; j++)
        {
                for (i = 0; i < 4; i++)
                        a[j] += c[i][j];
                a[j] = t[j] - a[j];
		if (a[j] >= 0)
	      		mvaddch(1, l, a[j] + 48);
		else
		{
			mvaddstr(LINES/2, COLS/2, "ERROR! A IS BIGGER THAN T!");
			refresh();
			sleep (4);
			endwin();
			return (0);
		}
                refresh();
                l += 3;
        }
	
	i = 3;
	j = 32;
	move(i, j);
	while ((ch = getch()) != 'q')
        {
                switch (ch)
                {
                case KEY_UP:
                        if (i > 3)
                                i--;
                        break;
                case KEY_RIGHT:
                        if (j < 38)
                                j += 2;
                        break;
                case KEY_LEFT:
                        if (j > 32)
                                j -= 2;
                        break;
                case KEY_DOWN:
                        if (i < 6)
                                i++;
                        break;
                default:
                        break;
                }
                move(i, j);
                if (ch >= '0' && ch <= '9')
                {
                        r[i - 3][(j - 32) / 2 ] = ch - 48;
                        mvaddch(i, j, r[i - 3][(j - 32) / 2] + 48);
		}
		refresh();
	}
        for (int k = 0; k < 4; k++)
        {
                for (i = 0; i < 4; i++)
                {
                        b = 0;
			chet = 0;
                        for (j = 0; j < 4; j++)
                        {
                                if (a[j] >= r[i][j])
                                        b++;
				if (c[i][j] == 0 && r[i][j] == 0 && stroka[i] == 0)
					chet++;
                        }
			if (chet == 4)
			{
				stroka[i] = 1;
			}
                        if (b == 4 && stroka[i] == 0)
                        {
                                for (int h = 0; h < 4; h++)
                                {
                                        r[i][h] = 0;
					mvaddch(i + 3, 32 + h * 2, r[i][h] + 48);
					refresh();
					sleep (1);
					a[h] += c[i][h];
					c[i][h] = 0;
                               	        mvaddch(i + 3, 11 + h * 2, c[i][h] + 48);
                               	        refresh();
                               	        sleep (1);
					mvaddch(1, 30 + h * 3, a[h] + 48);
					refresh();
					sleep (1);
				
                                }
				stroka[i] = 1;
                        }
                }
        }
	chet = 0;
	for (i = 0; i < 4; i++)
	{
		if (stroka[i] == 1)
			chet++;
	}
	if (chet == 4)
		mvaddstr(LINES/2, COLS/2, "EVERITHING IS ALRIGHT");
	else
		mvaddstr(LINES/2, COLS/2, "ERROR!");
	refresh();
	getch();	
        endwin();
        return (0);
}

