#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define ERROR_CREATE_THREAD -11
#define	SUCCES 0
//#define NUM_TREADS 8

pthread_mutex_t mutex;
int	bul = 1;

typedef	struct	someArgs_tag {
	int	i;
	int 	j;
	int	pos;
	char	simb;
}		someArgs_t;

void*	bullet(void *args)
{
	someArgs_t *arg = (someArgs_t*) args;
	int i;
	int j;

	pthread_mutex_lock(&mutex);
	i = arg->i;
	j = arg->j;
	if (arg->pos == 4 && bul)
	{
		bul = 0;
		while (j < COLS)
		{
			j += 1;
			mvaddch(i, j, '*');
			refresh();
			usleep(500000);
			mvaddch(i, j, ' ');
	//		getch();
		//	nodelay(stdscr, TRUE);
		}
		bul = 1;
	}
	pthread_mutex_unlock(&mutex);
	return	SUCCES;
}

int wheel(int x, int y, int k)
{
	if(k==1)
	{
		clear();
		mvaddch(x-1,y-1,'#');
		mvaddch(x-1,y,'#');
		mvaddch(x-1,y+1,'#');
		mvaddch(x,y,'o');
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
		mvaddch(x,y,'o');
		mvaddch(x,y+1,'#');
		mvaddch(x+1,y-1,'#');
		mvaddch(x+1,y+1,'#');
	}
}

int main(int argc, char* argv[])
{
	pthread_mutex_init(&mutex, NULL);
	initscr();
	pthread_mutex_unlock(&mutex);
	int ch, x, y, t, k, li, lj;
	int status, status_addr;
	pthread_t	thread;
	someArgs_t	args;

	x=LINES/2;
	y=COLS/2;
	k = 1;
	wheel(x,y,k);
	mvaddch(x,y+1,'-');
	args.i = x;
	args.j = y+1;
	args.simb = '-';
	keypad(stdscr,TRUE);
	noecho();
	while((ch=getch())!='i')
	{
		if(ch=='q')
                {
                        args.i = x - 1;
                        args.j = y - 1;
                        args.simb = '\\';
			args.pos = 1;
                }
                if(ch=='w')
                {
                        args.i = x - 1;
                        args.j = y;
                        args.simb = '|';
			args.pos = 2;
                }
                if(ch=='e')
                {
                        args.i = x - 1;
                        args.j = y + 1;
                        args.simb = '/';
			args.pos = 3;
                }
                if(ch=='d')
                {
                        args.i = x;
                        args.j = y + 1;
                        args.simb = '-';
			args.pos = 4;
                }

		if(ch=='c')
                {
                        args.i = x + 1;
                        args.j = y + 1;
                        args.simb = '\\';
			args.pos = 5;
                }
                if(ch=='x')
                {
                        args.i = x + 1;
                        args.j = y;
                        args.simb = '|';
			args.pos = 6;
                }
                if(ch=='z')
                {
                        args.i = x + 1;
                        args.j = y - 1;
                        args.simb = '/';
			args.pos = 7;
                }
                if(ch=='a')
                {
                        args.i = x;
                        args.j = y - 1;
                        args.simb = '-';
			args.pos = 8;
                }
		if (ch == 's')
		{
			status = pthread_create(&thread, NULL, bullet,(void*)  &args);
			if (status != 0)
				exit(ERROR_CREATE_THREAD);
		/*	else
			{
				status = pthread_join(thread,(void**)&status_addr);
			}*/
		}
		if(ch==KEY_DOWN && x + 1 != LINES)
                {
                        x+=1;
                        k=0;
                        args.i += 1;
                }
		if(ch==KEY_UP && x - 1 != 0)
                {
                        x-=1;
                        k=0;
                        args.i -= 1;
                }
                if(ch==KEY_LEFT && y - 1 != 0)
                {
                        y-=1;
                        k=1;
                        args.j -= 1;
                }
                if(ch==KEY_RIGHT && y + 1 != COLS)
                {
                        y+=1;
                        k=1;
                        args.j += 1;
                }
                wheel(x, y, k);
                mvaddch(args.i, args.j, args.simb);
		refresh();

	}
	refresh();
	getch();
	endwin();
	return 0;
}

