#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int wheel(int x,int y,int k)
{
	if(k==1)
	{
		clear();
		mvaddch(x-1,y-1,'#');
		mvaddch(x-1,y,'#');
		mvaddch(x-1,y+1,'#');
		mvaddch(x,y,'*');
		mvaddch(x+1,y-1,'#');
		mvaddch(x+1,y,'#');
		mvaddch(x+1,y+1,'#');
	}
	else
	{
		clear();
		mvaddch(x-1,y-1,'#');
		mvaddch(x-1,y+1,'#');
		mvaddch(x,y-1,'#');
		mvaddch(x,y,'*');
		mvaddch(x,y+1,'#');
		mvaddch(x+1,y-1,'#');
		mvaddch(x+1,y+1,'#');
	}
}

int main(int argc, char* argv[])
{
	initscr();
	int ch, x, y, t, k, li, lj;
	char simb;

	x=LINES/2;
	y=COLS/2;
	wheel(x,y,1);
	mvaddch(x,y+1,'-');
	li = x;
	lj = y + 1;
	simb = '-';
	keypad(stdscr,TRUE);
	noecho();
	while((ch=getch())!='i')
	{
		if(ch=='q')
		{
			li = x - 1;
			lj = y - 1;
			simb = '\\';
		}
		if(ch=='w')
		{
			li = x - 1;
			lj = y;
			simb = '|';
		}
		if(ch=='e')
		{
			li = x - 1;
			lj = y + 1;
			simb = '/';
		}
		if(ch=='d')
		{
			li = x;
			lj = y + 1;
			simb = '-';
		}
		if(ch=='c')
		{
			li = x + 1;
			lj = y + 1;
			simb = '\\';
		}
		if(ch=='x')
		{
			li = x + 1;
			lj = y;
			simb = '|';
		}
		if(ch=='z')
		{
			li = x + 1;
			lj = y - 1;
			simb = '/';
		}
		if(ch=='a')
		{
			li = x;
			lj = y - 1;
			simb = '-';
		}
		if(ch==KEY_DOWN && x + 1 != LINES)
		{
			x+=1;
			k=0;
			li += 1;
		}
		if(ch==KEY_UP && x - 1 != 0)
		{
			x-=1;
			k=0;
			li -= 1;
		}
		if(ch==KEY_LEFT && y - 1 != 0)
		{
			y-=1;
			k=1;
			lj -= 1;
		}
		if(ch==KEY_RIGHT && y + 1 != COLS)
		{
			y+=1;
			k=1;
			lj += 1;
		}
		wheel(x, y, k);
		mvaddch(li, lj, simb);
	}
	refresh();
	getch();
	endwin();
	return 0;
}


