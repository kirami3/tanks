#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<ncurses.h>
#include<stdlib.h>
#define MAX_BULLETS 3
#define SHIP_STEP 100000
#define SHIP_LEN 4
#define BULLET_STEP 50000
int bc=0,sy;

void *ship(void *args){
    //vars init
    int p=LINES+LINES/2,i;
    //floating
    while(1){
        p--;
        if(p==LINES){p=2*LINES;}
        sy=p;
        for(i=0;i<SHIP_LEN;i++){
            mvaddch((p+i)%LINES,COLS-2,'#');
        }
        mvaddch((p+SHIP_LEN)%LINES,COLS-2,' ');
        usleep(SHIP_STEP);
    }
}

void *bullet(void *args){
    //vars init
    int i,x,y=LINES/2;
    //flying
    for(x=5;x<COLS-1;x++){
        //draw it
        mvaddch(y,x,'*');
        //clean 2 cells behind
        mvaddch(y,x-1,' ');
        mvaddch(y,x-2,' ');
        usleep(BULLET_STEP);
    }
    //check if hit
    if((sy%LINES-LINES/2<=0)&&(LINES/2-sy%LINES<SHIP_LEN)){
        endwin();
        exit(0);
    }
    //clean its path
    for(i=3;i<COLS-1;i++){
        mvaddch(y,i,' ');
    }
    bc--;
    return 0;
}

int main(){
    //init variables
    pthread_t st,bt[3];
    int retval,key;
    //init graphics
    initscr();
    keypad(stdscr,TRUE);
    noecho();
    nodelay(stdscr,TRUE);
    //start ships thread
    pthread_create(&st,NULL,ship,NULL);
    //drawing gun
    mvprintw(LINES/2,0,"===");
    //register shot
    while((key=getch())!=KEY_END){
        if((key==' ')&&(bc<3)){
            bc++;
            pthread_create(&bt[bc-1],NULL,bullet,NULL);
            usleep(100000);
        }
    }
    endwin();
    return 0;
}

