#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <ncurses.h>

int main(int argc, char* argv[]){
	
	int ch, mo;
	int i;
	int l, c;
	int sost;
	
	initscr();
	noecho();
	keypad(stdscr, TRUE);
	l = LINES;
	c = COLS;
	srand(l);
	srand(c);
	int x, y, xh, yh;
	x = LINES/2;
	y = COLS/2;
	sost = 1;
	mvaddch(x, y, '*');
	mvaddch(0, 0, '@');
	refresh();
	while ((ch = getch())!= '`'){
		clear();
		if (sost == 1){
		if (ch == 'q'){
			xh = -1;
            yh = -1;
            mo = 'q';
		}
		if (ch == 'w'){
			xh = -1;
            yh = 0;
            mo = 'w';
		}
		if (ch == 'e'){
			xh = -1;
            yh = 1;
            mo = 'e';
		}
		if (ch == 'a'){
			xh = 0;
            yh = -1;
            mo = 'a';
		}
		if (ch == 'd'){
			xh = 0;
            yh = 1;
            mo = 'd';
		}
		if (ch == 'z'){
			xh = 1;
            yh = -1;
            mo = 'z';
		}
		if (ch == 'x'){
			xh = 1;
            yh = 0;
            mo = 'x';
		}
		if (ch == 'c'){
			xh = 1;
            yh = 1;
            mo = 'c';
		}}
		if (sost == -1){
		if (ch == 'c'){
			xh = -1;
            yh = -1;
            mo = 'q';
		}
		if (ch == 'x'){
			xh = -1;
            yh = 0;
            mo = 'w';
		}
		if (ch == 'z'){
			xh = -1;
            yh = 1;
            mo = 'e';
		}
		if (ch == 'd'){
			xh = 0;
            yh = -1;
            mo = 'a';
		}
		if (ch == 'a'){
			xh = 0;
            yh = 1;
            mo = 'd';
		}
		if (ch == 'e'){
			xh = 1;
            yh = -1;
            mo = 'z';
		}
		if (ch == 'w'){
			xh = 1;
            yh = 0;
            mo = 'x';
		}
		if (ch == 'q'){
			xh = 1;
            yh = 1;
            mo = 'c';
		}
		}
		if (((x+xh)<0) || ((x+xh) > LINES) || ((y+yh)<0) || ((y+yh)>COLS)){
		if (mo == 'q'){
			xh = 1;
            yh = 1;
		}
		if (mo == 'w'){
			xh = 1;
            yh = 0;
		}
		if (mo == 'e'){
			xh = 1;
            yh = -1;
		}
		if (mo == 'a'){
			xh = 0;
            yh = 1;
		}
		if (mo == 'd'){
			xh = 0;
            yh = -1;
		}
		if (mo == 'z'){
			xh = -1;
            yh = 1;
		}
		if (mo == 'x'){
			xh = -1;
            yh = 0;
		}
		if (mo == 'c'){
			xh = -1;
            yh = -1;
		}	
		sost = sost*(-1);
		
	}
    	mvaddch(x+xh, y+yh, '*');
    	x = x+xh; y = y+yh;
		refresh();
	}
	end:
	endwin();
	return 0;
}
