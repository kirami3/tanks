// gcc name -lncurses
//Press F to exit from program
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>


int wheel(x, y, k)
{
    if(k==1)
    {
        clear();
        mvaddch(x,y,'@');
    }
    else
    {
        clear();
    }
}


int main(int argc, char* argv[])
{
    initscr();
    int ch; int x; int y; int t; int k;
   
    x=LINES/2;
    y=COLS/2;
    k=0;
    
    wheel(x,y,k);
    
    noecho();
    keypad(stdscr,TRUE);
    
    clear();
    
    while((ch=getch())!='f'){
        clear();
        
        if(ch == 'q'){
            x-=1;
            y-=1;
            if (x == 0){
                x++;
                x++;
            }
            if (y == 0){
                y++;
                y++;
            }
            k=1;
            wheel(x,y,k);
            //mvaddch(x-1,y-1,'@');
            getch();
        }
        if(ch == 'e')
        {
            x-=1;
            y+=1;
            if (x == 0){
                x++;
                x++;
            }
            if (y == COLS){
                y--;
                y--;
            }
            k=1;
            wheel(x,y,k);
            //mvaddch(x-1,y+1,'@');
            getch();
        }
        if(ch == 'c')
        {
            x+=1;
            y+=1;
            if (x == LINES){
                x--;
                x--;
            }
            if (y == COLS){
                y--;
                y--;
            }
            k=1;
            wheel(x,y,k);
            //mvaddch(x+1,y+1,'@');
            getch();
        }
        if(ch == 'z')
        {
            x+=1;
            y-=1;
            if (x == LINES){
                x--;
                x--;
            }
            if (y == COLS){
                y++;
                y++;
            }
            k=1;
            wheel(x,y,k);
            //mvaddch(x+1,y+1,'@');
            getch();
        }
        
        
        
        if(ch == 'x')
        {
            x+=1;
            if (x == LINES){
                x--;
                x--;
            }
            k=1;
            wheel(x,y,k);
            //mvaddch(x+1,y,'@');
            getch();
        }
        if(ch == 'w')
        {
            x-=1;
            if (x == 0){
                x++;
                x++;
            }
            k=1;
            wheel(x,y,k);
            //mvaddch(x-1,y,'@');
            getch();
        }
        if(ch == 'a')
        {
            y-=1;
            if (y == 0){
                y++;
                y++;
            }
            k=1;
            wheel(x,y,k);
            //mvaddch(x,y-1,'@');
            getch();
        }
        if(ch == 'd')
        {
            y+=1;
            if (y == COLS){
                y--;
                y--;
            }
            k=1;
            wheel(x,y,k);
            //mvaddch(x,y+1,'@');
            getch();
        }
    }
    refresh();
    getch();
    endwin();
    return 0;
}
