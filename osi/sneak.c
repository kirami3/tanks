#include <unistd.h>
#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct	s_list
{
	struct s_list *next;
	int	*data1;
	int	*data2;
}		t_list;

t_list *ft_create(int data1, int data2)
{
	t_list *tmp;

	if ((tmp = (t_list *)malloc(sizeof(t_list))) == NULL)
		return (NULL);
	tmp->data1 = data1;
	tmp->data2 = data2;
	return (tmp);
}

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
	t_list *list;
	t_list *chto;
	initscr();
start:
	i = LINES/2;
	j = COLS/2;
	mvaddstr(i, j, "\n\tHello, this is sneak.\n\t You can control sneak with your keypad.\n\t Tipe something, how many food do you like, ENTER and we will start.");
	getch();
	refresh();
	clear();
	refresh();
	scanf("%d", &h);
	chet = h;
        gi = (int*)malloc(4 * h);
        gj = (int*)malloc(4 * h);
        srand(time(NULL));
        for (int m = 0; m < h; m++)
        {
                gi[m] = rand() % 28;
                gj[m] = rand() % 78;
                while ((gj[m] == 36 || gj[m] == 37 || gj[m] == 38 || gj[m] == 39) && gi[m] == 14)
                        gj[m] = rand() % 78;
                mvaddch(gi[m], gj[m], '#');
        }
        refresh();
	timeout (1);
	keypad(stdscr, TRUE);
	curs_set(0);
	list = ft_create(i, j);
	list->next = NULL;
	for (t = 0; t < 3; t++)
	{
		j--;
		chto = ft_create(i, j);
		chto->next = list;
		list = chto;
	}
	j = COLS/2;
	v = '>';
	t = 0;
	while (chet)
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
		for (int m = 0; m < h; m++)
		{
			if (i == gi[m] && j == gj[m])
			{
				gi[m] = -1;
				gj[m] = -1;
				chto = ft_create(list->data1, list->data2);
				chto->next = list;
				list = chto;
				chet--;
			}
		}
		chto = list;
		mvaddch(chto->data1, chto->data2, ' ');
		while (chto->next != NULL)
		{
			if(t)
			{
				chto->data1 = chto->next->data1;
				chto->data2 = chto->next->data2;
			}
			mvaddch(chto->data1, chto->data2, '*');
			chto = chto->next;
		}
		chto->data1 = i;
		chto->data2 = j;
		mvaddch(chto->data1, chto->data2, v);
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

