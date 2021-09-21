#include <pthread.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#ifndef PORT
#  define PORT 9999
#endif
#define ARMORE '#'
#define BULLET '0'
#define BULLET_SPEED 50000
struct body
{
	int x,y;
};

struct player
{
	struct body gusly[3][3];
	struct body bullet;
	int pos;
	int dir;
	int fire_flag;
	int rotc;
};

struct params_for_udp_server
{
  char* ip1;
  char* ip2;
  struct player* tank1;
  struct player* tank2;
};

struct params_for_movet
{
	int* key;
	struct player* tank;
};

struct params_for_draw
{
	struct player* tank1;
	struct player* tank2;
};

pthread_mutex_t mutex;

int check_key(int key);
void rotate_canon(struct player* tank,int pos);
void rotate_gusly(struct player* tank,int dir);
void set_tank(struct player* tank,int y,int x,int pos,int dir,int rotc,int fire_flag);
void move_tank(struct player* tank,int* ch);
void* draw(void* param);
void* shoot(void* param);
void* movet(void* tank);
void* rules(void* param);
int key1;
int key2;
int game=0;

int work_flag = 1;
void* udp_server(void* param)
{
  int sock, addr_len, count, ret;
  char buffer[4];
  fd_set readfd;
  struct sockaddr_in addr;
  
 
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if ( 0 > sock ) {
    perror("socket");
    return NULL;
  }

  addr_len = sizeof(struct sockaddr_in);
  memset((void*)&addr, 0, addr_len);
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htons(INADDR_ANY);
  addr.sin_port = htons(PORT);

  if ( bind(sock,(struct sockaddr*)&addr, addr_len) < 0) {
    perror("bind");
    close(sock);
    return NULL;
  }
  
  //char* info;
  struct params_for_udp_server* par=(struct params_for_udp_server*)param;
  while (work_flag) {
    FD_ZERO(&readfd);
    FD_SET(sock, &readfd);

    ret = select(sock+1, &readfd, NULL, NULL, 0);
    if (ret > 0) {
      if (FD_ISSET(sock, &readfd)) {
        count = recvfrom(sock, buffer, 4, 0, (struct sockaddr*)&addr, &addr_len);
        if(strcmp(par->ip1,inet_ntoa(addr.sin_addr))==0)
        {
          key1=*((int*)buffer);   
		  move_tank(par->tank1,&key1);
        }
        else if(strcmp(par->ip2,inet_ntoa(addr.sin_addr))==0)
        {
          key2=*((int*)buffer);   
		  move_tank(par->tank2,&key2);
        }
        
      }
    }
  }
  close(sock);
  return (void*)NULL;
}

int main(int argc, char *argv[])
{
  if(argc!=3)
  {
    printf("LOH\n");
    return 0;
  }
  int sock, yes = 1;
  pthread_t pid,sh1,sh2,dr,mv1,mv2,ru;
  struct sockaddr_in addr;
  int addr_len;
  int count;
  int ret;
  fd_set readfd;
  char buffer[1024];
  int i;
  struct params_for_udp_server players;
  players.ip1=argv[1];
  players.ip2=argv[2];
  pthread_mutex_init(&mutex,NULL);
  initscr();
  curs_set(0);
  noecho();
  keypad(stdscr,TRUE);
  start_color();
  init_pair(1,COLOR_RED,COLOR_BLACK);
  init_pair(2,COLOR_BLUE,COLOR_BLACK);
  struct player tank1,tank2;
  tank1.bullet.y=-1;
  tank1.bullet.x=-1;
  tank2.bullet.y=-1;
  tank2.bullet.x=-1;
  clear();
  set_tank(&tank1,LINES/2,10,1,1,0,0);
  set_tank(&tank2,LINES/2,COLS-1-10,1,1,0,0);
  refresh();
  struct params_for_draw dra;
  dra.tank1=&tank1;
  dra.tank2=&tank2;
  struct params_for_movet t1,t2;
  t1.key=&key1;
  t2.key=&key2;
  players.tank1=&tank1;
  players.tank2=&tank2;
  t1.tank=&tank1;
  t2.tank=&tank2;
  pthread_create(&pid, NULL,udp_server,(void*)&players);

  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0) {
    perror("sock");
    return -1;
  }

  ret = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&yes, sizeof(yes));
  if (ret == -1) {
    perror("setsockopt");
    return 0;
  }

  addr_len = sizeof(struct sockaddr_in); 
  memset((void*)&addr, 0, addr_len);
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
  addr.sin_port = htons(PORT);

  int key;
  pthread_create(&ru,NULL,rules,(void*)&dra);
  pthread_create(&dr,NULL,draw,(void*)&dra);
  //pthread_create(&mv1,NULL,movet,(void*)&t1);
  //pthread_create(&mv2,NULL,movet,(void*)&t2);
  pthread_create(&sh1,NULL,shoot,(void*)&tank1);
  pthread_create(&sh2,NULL,shoot,(void*)&tank2);
  
  while(work_flag) {
    key=getch();
    sendto(sock, &key,4 , 0, (struct sockaddr*) &addr, addr_len);
  }
  if(pthread_join(pid, NULL)<0)
  {
	  fprintf(stderr,"pid\n");
  }
  if(pthread_join(dr,NULL)<0)
  {
	  fprintf(stderr,"dr\n");
  }
  /*if(pthread_join(mv1,NULL)<0)
  {
	  fprintf(stderr,"mv1\n");
  }
  if(pthread_join(mv2,NULL)<0)
  {
	  fprintf(stderr,"mv2\n");
  }*/
  if(pthread_join(sh1,NULL)<0)
  {
	  fprintf(stderr,"sh1\n");
  }
  if(pthread_join(sh2,NULL)<0)
  {
	  fprintf(stderr,"sh2\n");
  }
  if(pthread_join(ru,NULL)<0)
  {
	  fprintf(stderr,"ru\n");
  }
  getch();
  endwin();
  close(sock);
  return 0;
}

void rotate_canon(struct player* tank,int pos)
{
	switch(pos)
	{
		case 0:
			mvaddch(tank->gusly[0][0].y,tank->gusly[0][0].x,'\\');
			tank->pos=0;
			break;
		case 1:
			mvaddch(tank->gusly[0][1].y,tank->gusly[0][1].x,'|');
			tank->pos=1;
			break;
		case 2:
			mvaddch(tank->gusly[0][2].y,tank->gusly[0][2].x,'/');
			tank->pos=2;
			break;
		case 3:
			mvaddch(tank->gusly[1][0].y,tank->gusly[1][0].x,'-');
			tank->pos=3;
			break;
		case 4:
			mvaddch(tank->gusly[1][2].y,tank->gusly[1][2].x,'-');
			tank->pos=4;
			break;
		case 5:
			mvaddch(tank->gusly[2][0].y,tank->gusly[2][0].x,'/');
			tank->pos=5;
			break;
		case 6:
			mvaddch(tank->gusly[2][1].y,tank->gusly[2][1].x,'|');
			tank->pos=6;
			break;
		case 7:
			mvaddch(tank->gusly[2][2].y,tank->gusly[2][2].x,'\\');
			tank->pos=7;
			break;
		default:
			break;
	}
}

void rotate_gusly(struct player* tank,int dir)
{
	switch(dir)
	{
		case 0:
			mvaddch(tank->gusly[0][1].y,tank->gusly[0][1].x,ARMORE);
			mvaddch(tank->gusly[2][1].y,tank->gusly[2][1].x,ARMORE);
			mvaddch(tank->gusly[1][0].y,tank->gusly[1][0].x,' ');
			mvaddch(tank->gusly[1][2].y,tank->gusly[1][2].x,' ');
			tank->dir=0;
			break;// left/right
		case 1:
			mvaddch(tank->gusly[0][1].y,tank->gusly[0][1].x,' ');
			mvaddch(tank->gusly[2][1].y,tank->gusly[2][1].x,' ');
			mvaddch(tank->gusly[1][0].y,tank->gusly[1][0].x,ARMORE);
			mvaddch(tank->gusly[1][2].y,tank->gusly[1][2].x,ARMORE);
			tank->dir=1;
			break;// up/down
		default:
			return;
			break;
	}
	mvaddch(tank->gusly[0][0].y,tank->gusly[0][0].x,ARMORE);
	mvaddch(tank->gusly[0][2].y,tank->gusly[0][2].x,ARMORE);
	mvaddch(tank->gusly[1][1].y,tank->gusly[1][1].x,ARMORE);
	mvaddch(tank->gusly[2][0].y,tank->gusly[2][0].x,ARMORE);
	mvaddch(tank->gusly[2][2].y,tank->gusly[2][2].x,ARMORE);
}

void set_tank(struct player* tank,int y,int x,int pos,int dir,int rotc,int fire_flag)
{
	if(!rotc)
	{
		tank->gusly[1][1].y=y;
		tank->gusly[1][1].x=x;
		tank->gusly[0][0].y=tank->gusly[1][1].y-1;
		tank->gusly[0][0].x=tank->gusly[1][1].x-1;
		tank->gusly[0][1].y=tank->gusly[1][1].y-1;
		tank->gusly[0][1].x=tank->gusly[1][1].x;
		tank->gusly[0][2].y=tank->gusly[1][1].y-1;
		tank->gusly[0][2].x=tank->gusly[1][1].x+1;
		tank->gusly[1][0].y=tank->gusly[1][1].y;
		tank->gusly[1][0].x=tank->gusly[1][1].x-1;
		tank->gusly[1][2].y=tank->gusly[1][1].y;
		tank->gusly[1][2].x=tank->gusly[1][1].x+1;
		tank->gusly[2][0].y=tank->gusly[1][1].y+1;
		tank->gusly[2][0].x=tank->gusly[1][1].x-1;
		tank->gusly[2][1].y=tank->gusly[1][1].y+1;
		tank->gusly[2][1].x=tank->gusly[1][1].x;
		tank->gusly[2][2].y=tank->gusly[1][1].y+1;
		tank->gusly[2][2].x=tank->gusly[1][1].x+1;
		tank->dir=dir;
	}//смысл менять координаты,когда нужно только повернуть пушку? 
	tank->pos=pos;
	tank->fire_flag=fire_flag;
	rotate_gusly(tank,tank->dir);
	rotate_canon(tank,tank->pos);
	if(tank->bullet.y!=-1 && tank->bullet.x!=-1)
	{
		mvaddch(tank->bullet.y,tank->bullet.x,BULLET);
	}
}

void move_tank(struct player* tank,int* ch)
{
	int rotc=0;
	switch(*ch)
	{
		case 'q':
			tank->pos=0;
			tank->rotc=1;
			break;
		case 'w':
			tank->pos=1;
			tank->rotc=1;
			break;
		case 'e':
			tank->pos=2;
			tank->rotc=1;
			break;
		case 'a':
			tank->pos=3;
			rotc=1;
			break;
		case 'd':
			tank->pos=4;
			tank->rotc=1;
			break;
		case 'z':
			tank->pos=5;
			rotc=1;
			break;
		case 'x':
			tank->pos=6;
			tank->rotc=1;
			break;
		case 'c':
			tank->pos=7;
			tank->rotc=1;
			break;
		case KEY_UP:
			if((tank->gusly[1][1].y-2)>=0)
			{
				tank->gusly[1][1].y--;
				tank->dir=1;
				tank->rotc=0;
			}
			break;
		case KEY_DOWN:
			if((tank->gusly[1][1].y+2)<=(LINES-1))
			{
				tank->gusly[1][1].y++;
				tank->dir=1;
				tank->rotc=0;
			}
			break;
		case KEY_LEFT:
			if((tank->gusly[1][1].x-2)>=0)
			{
				tank->gusly[1][1].x--;
				tank->dir=0;
				tank->rotc=0;
			}
			break;
		case KEY_RIGHT:
			if((tank->gusly[1][1].x+2)<=(COLS-1))
			{
				tank->gusly[1][1].x++;
				tank->dir=0;
				tank->rotc=0;
			}
			break;
		case 's':
			if(tank->bullet.x==-1 && tank->bullet.y==-1)
			{
				tank->fire_flag=1;
			}
			break;
		default:
			break;	
	}
	if(*ch==KEY_F(1))
	{

	}
	else
	{
		*ch=0;
	}
	
	//set_tank(tank,tank->gusly[1][1].y,tank->gusly[1][1].x,tank->pos,tank->dir,rotc,tank->fire_flag);
}

void* draw(void* param)
{
	struct params_for_draw* pl=(struct params_for_draw*)param;
	while(work_flag)
	{
		//pthread_mutex_lock(&mutex);
		clear();
		attron(COLOR_PAIR(1));
		set_tank(pl->tank1,pl->tank1->gusly[1][1].y,pl->tank1->gusly[1][1].x,pl->tank1->pos,pl->tank1->dir,pl->tank1->rotc,pl->tank1->fire_flag);
		attroff(COLOR_PAIR(1));
		attron(COLOR_PAIR(2));
		set_tank(pl->tank2,pl->tank2->gusly[1][1].y,pl->tank2->gusly[1][1].x,pl->tank2->pos,pl->tank2->dir,pl->tank2->rotc,pl->tank2->fire_flag);
		attroff(COLOR_PAIR(2));
		switch(game)
		{
			case 1:
				attron(COLOR_PAIR(1));
				mvaddstr(LINES/2,COLS/2-strlen("Player1 wins")/2,"Player1 wins");
				attroff(COLOR_PAIR(1));
				work_flag=0;
				break;
			case 2:
				attron(COLOR_PAIR(2));
				mvaddstr(LINES/2,COLS/2-strlen("Player2 wins")/2,"Player2 wins");
				attroff(COLOR_PAIR(2));
				work_flag=0;
				break;
			case 3:
				break;
			case 4:
				break;
			default: 
				break;
		}
		refresh();
		//pthread_mutex_unlock(&mutex);
		usleep(40000);
	}
}

void* shoot(void* param)
{
	int dx,dy;
	struct player* tank=(struct player*)param;
	while(work_flag)
	{
		//pthread_mutex_lock(&mutex);
		if(tank->fire_flag==1)
		{
			if(tank->bullet.x==-1 && tank->bullet.y==-1)
			{
				switch(tank->pos)
				{
					case 0:
						tank->bullet.y=tank->gusly[0][0].y-1;
						tank->bullet.x=tank->gusly[0][0].x-1;
						dx=-1;
						dy=-1;
						break;
					case 1:
						tank->bullet.y=tank->gusly[0][1].y-1;
						tank->bullet.x=tank->gusly[0][1].x;
						dx=0;
						dy=-1;
						break;
					case 2:
						tank->bullet.y=tank->gusly[0][2].y-1;
						tank->bullet.x=tank->gusly[0][2].x+1;
						dx=1;
						dy=-1;
						break;
					case 3:
						tank->bullet.y=tank->gusly[1][0].y;
						tank->bullet.x=tank->gusly[1][0].x-1;	
						dy=0;
						dx=-1;
						break;
					case 4:
						tank->bullet.y=tank->gusly[1][2].y;
						tank->bullet.x=tank->gusly[1][2].x+1;
						dy=0;
						dx=1;	
						break;
					case 5:
						tank->bullet.y=tank->gusly[2][0].y+1;
						tank->bullet.x=tank->gusly[2][0].x-1;	
						dy=1;
						dx=-1;
						break;
					case 6:
						tank->bullet.y=tank->gusly[2][1].y+1;
						tank->bullet.x=tank->gusly[2][1].x;	
						dy=1;
						dx=0;
						break;
					case 7:
						tank->bullet.y=tank->gusly[2][2].y+1;
						tank->bullet.x=tank->gusly[2][2].x+1;	
						dy=1;
						dx=1;
						break;
				}
			}
			if(tank->bullet.y<0 || tank->bullet.y>LINES-1 || tank->bullet.x<0 || tank->bullet.x>COLS-1)
			{
				tank->fire_flag=0;
				tank->bullet.y=-1;
				tank->bullet.x=-1;
				dx=0;
				dy=0;
			}
			else
			{
				tank->bullet.y+=dy;
				tank->bullet.x+=dx;
				
			}
		}
		//pthread_mutex_unlock(&mutex);
		usleep(BULLET_SPEED);
	}
}

void* rules(void* param)
{	
	int exit=0;
	struct params_for_draw* pl=(struct params_for_draw*)param;
	while(work_flag)
	{
		pthread_mutex_lock(&mutex);
		for(int i=0;i<3;i++)
		{
			if(work_flag==0) break;
			for(int j=0;j<3;j++)
			{
				if(pl->tank1->bullet.y==pl->tank2->gusly[i][j].y && pl->tank1->bullet.x==pl->tank2->gusly[i][j].x)
				{
					game=1;
					exit=1;
					break;
				}
				else if(pl->tank2->bullet.y==pl->tank1->gusly[i][j].y && pl->tank2->bullet.x==pl->tank1->gusly[i][j].x)
				{
					game=2;
					exit=1;
					break;
				}
				if(exit)
				{
					break;
				}
			}
		}
		if(pl->tank1->bullet.y!=-1 && pl->tank2->bullet.y!=-1 && pl->tank1->bullet.x!=-1 && pl->tank2->bullet.x!=-1)
		{
			if(pl->tank1->bullet.y==pl->tank2->bullet.y && pl->tank1->bullet.x==pl->tank2->bullet.x)
			{
				pl->tank1->bullet.y=-1;
				pl->tank1->bullet.x=-1;
				pl->tank2->bullet.y=-1;
				pl->tank2->bullet.x=-1;
				pl->tank2->fire_flag=0;
				pl->tank1->fire_flag=0;
			}
		}
		if(key1==KEY_F(1)) game=3;
		if(key2==KEY_F(1)) game=4;
		pthread_mutex_unlock(&mutex);
	}
}