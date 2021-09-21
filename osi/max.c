#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <pthread.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

void you_killed(){
	mvaddstr(LINES/2-5, COLS/2, "You are killed!");
	refresh();
	while(1)
		usleep(1000);
}

void dulo_w(int x, int y){
	mvaddch(y-1,x-1,'*');
	mvaddch(y-1,x,'|');
	mvaddch(y-1,x+1,'*');
	mvaddch(y,x-1,'*');
	mvaddch(y,x,'o');
	mvaddch(y,x+1,'*');
	mvaddch(y+1,x-1,'*');
	mvaddch(y+1,x,'*');
	mvaddch(y+1,x+1,'*');
}


void dulo_e(int x, int y){
	mvaddch(y-1,x-1,'*');
	mvaddch(y-1,x,'*');
	mvaddch(y-1,x+1,'/');
	mvaddch(y,x-1,'*');
	mvaddch(y,x,'o');
	mvaddch(y,x+1,'*');
	mvaddch(y+1,x-1,'*');
	mvaddch(y+1,x,'*');
	mvaddch(y+1,x+1,'*');
}

void dulo_d(int x, int y){
	mvaddch(y-1,x-1,'*');
	mvaddch(y-1,x,'*');
	mvaddch(y-1,x+1,'*');
	mvaddch(y,x-1,'*');
	mvaddch(y,x,'o');
	mvaddch(y,x+1,'-');
	mvaddch(y+1,x-1,'*');
	mvaddch(y+1,x,'*');
	mvaddch(y+1,x+1,'*');
}

void dulo_c(int x, int y){
	mvaddch(y-1,x-1,'*');
	mvaddch(y-1,x,'*');
	mvaddch(y-1,x+1,'*');
	mvaddch(y,x-1,'*');
	mvaddch(y,x,'o');
	mvaddch(y,x+1,'*');
	mvaddch(y+1,x-1,'*');
	mvaddch(y+1,x,'*');
	mvaddch(y+1,x+1,'\\');
}

void dulo_x(int x, int y){
	mvaddch(y-1,x-1,'*');
	mvaddch(y-1,x,'*');
	mvaddch(y-1,x+1,'*');
	mvaddch(y,x-1,'*');
	mvaddch(y,x,'o');
	mvaddch(y,x+1,'*');
	mvaddch(y+1,x-1,'*');
	mvaddch(y+1,x,'|');
	mvaddch(y+1,x+1,'*');
}

void dulo_z(int x, int y){
	mvaddch(y-1,x-1,'*');
	mvaddch(y-1,x,'*');
	mvaddch(y-1,x+1,'*');
	mvaddch(y,x-1,'*');
	mvaddch(y,x,'o');
	mvaddch(y,x+1,'*');
	mvaddch(y+1,x-1,'/');
	mvaddch(y+1,x,'*');
	mvaddch(y+1,x+1,'*');
}

void dulo_a(int x, int y){
	mvaddch(y-1,x-1,'*');
	mvaddch(y-1,x,'*');
	mvaddch(y-1,x+1,'*');
	mvaddch(y,x-1,'-');
	mvaddch(y,x,'o');
	mvaddch(y,x+1,'*');
	mvaddch(y+1,x-1,'*');
	mvaddch(y+1,x,'*');
	mvaddch(y+1,x+1,'*');
}

void dulo_q(int x, int y){
	mvaddch(y-1,x-1,'\\');
	mvaddch(y-1,x,'*');
	mvaddch(y-1,x+1,'*');
	mvaddch(y,x-1,'*');
	mvaddch(y,x,'o');
	mvaddch(y,x+1,'*');
	mvaddch(y+1,x-1,'*');
	mvaddch(y+1,x,'*');
	mvaddch(y+1,x+1,'*');
}

void set_color(int color_index){
	attron(COLOR_PAIR(color_index));
}

// состояние игрового поля (включает свой и чужой танк и координаты снаряда)
int center_x, center_y;// центр танка
int dulo=0;
int gun_x=-1, gun_y=-1;
int vrag_x=-1, vrag_y=-1, vrag_dulo=-1;
int vrag_gun_x=-1, vrag_gun_y=-1;

//FILE* tlog;

pthread_mutex_t mutex;
int fdx=0, fdy=0;

void draw_all(){
	//fprintf(tlog,"draw_all()\n");
	//fflush(tlog);

    pthread_mutex_lock(&mutex);

	// рисуем свой танк
	clear();
    set_color(1);
    switch (dulo) {
    case 0:
        dulo_w(center_x,center_y);
        break;
	
    case 1:
		dulo_e(center_x,center_y);
	    break;

	case 2:
		dulo_d(center_x,center_y);
	    break;

	case 3:
		dulo_c(center_x,center_y);
	    break;

	case 4:
		dulo_x(center_x, center_y);
    	break;

	case 5:
		dulo_z(center_x, center_y);
	    break;

	case 6:
		dulo_a(center_x, center_y);
    	break;

	case 7:
		dulo_q(center_x, center_y);
	    break;
    
    default:
        break;
    }
	// снаряд
    if (gun_x>=0 && gun_y>=0)
		mvaddch(gun_y,gun_x,'*');

    // рисуем вражий танк, если о нём есть информация
    if (vrag_x>0){
		//fprintf(tlog,"draw vrag tank\n");
		//fflush(tlog);

		
		set_color(2);
		switch (vrag_dulo) {
		case 0:
			dulo_w(vrag_x,vrag_y);
			break;
		
		case 1:
			dulo_e(vrag_x,vrag_y);
			break;

		case 2:
			dulo_d(vrag_x,vrag_y);
			break;

		case 3:
			dulo_c(vrag_x,vrag_y);
			break;

		case 4:
			dulo_x(vrag_x, vrag_y);
			break;

		case 5:
			dulo_z(vrag_x, vrag_y);
			break;

		case 6:
			dulo_a(vrag_x, vrag_y);
			break;

		case 7:
			dulo_q(vrag_x, vrag_y);
			break;
		
		default:
			break;
		}
			// вражеский снаряд
		if (vrag_gun_x>=0 && vrag_gun_y>=0)
			mvaddch(vrag_gun_y,vrag_gun_x,'*');		

	}
	
    refresh();

    pthread_mutex_unlock(&mutex);
}

int sock;
struct sockaddr_in addr;
unsigned int addr_len;
unsigned char buffer[10];

void send_coords(){
		buffer[0]=1;// тип пакета. других нет
		buffer[1] = center_x;
		buffer[2] = center_y;
		buffer[3]=dulo;
		buffer[4] = gun_x;
		buffer[5] = gun_y;
		sendto(sock, buffer, 6, 0, (struct sockaddr*) &addr, addr_len);
}

int received_packets_cnt=0;

void* udp_server(void *args){
	int sock, count, ret;
	unsigned int addr_len_2;
	char buffer[1024];
	fd_set readfd;
	struct sockaddr_in addr;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if ( 0 > sock ) {
		perror("socket");
		return NULL;
	}

	addr_len_2 = sizeof(struct sockaddr_in);
	memset((void*)&addr, 0, addr_len_2);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htons(INADDR_ANY);
	addr.sin_port = htons(6667);

	int one = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &one, sizeof(one));

	if ( bind(sock,(struct sockaddr*)&addr, addr_len_2) < 0) {
		perror("bind");
		close(sock);
		return NULL;
	}
	while (1) {
		FD_ZERO(&readfd);
		FD_SET(sock, &readfd);

		ret = select(sock+1, &readfd, NULL, NULL, 0);
		if (ret > 0) {
			if (FD_ISSET(sock, &readfd)) {
				count = recvfrom(sock, buffer, 1024, 0, (struct sockaddr*)&addr, &addr_len_2); 
				if ( buffer[0]==1 && (buffer[1]!=center_x || buffer[2]!=center_y) ){
					vrag_x = buffer[1];
					vrag_y = buffer[2];
					vrag_dulo=buffer[3];
					vrag_gun_x=buffer[4];
					vrag_gun_y=buffer[5];
					if (received_packets_cnt==0){
						// это - первый пакет о вражеском танке. В ответ надо послать координаты нашего танка
						usleep(100);
						send_coords();
					}
					received_packets_cnt++;
					// детектирование попадания
					int tx = abs(vrag_gun_x-center_x);
					int ty = abs(vrag_gun_y-center_y);

						

					if (tx<=1 && ty<=1){
						you_killed();
					}

					
				}

				draw_all();
			}
		}
	}
	close(sock);
	return (void*)NULL;
}

void* gun(void *args){
	while(1){
		usleep(50000);
		if ( gun_x>=0 && gun_y>=0 ){
			
			gun_x += fdx; gun_y += fdy;
			int flag=0;
			if ( gun_x<COLS/2-40 || gun_x>COLS/2+40 || gun_y<LINES/2-20 || gun_y>LINES/2+20 ) { 
				gun_x=-1; gun_y = -1;
				flag=1;
			}
			if ( (gun_x>=0 && gun_y>=0) || flag==1)
				send_coords();
		}
	}
	return (void*)NULL;
}

int main(){
    
	//tlog = fopen("tank3.log", "a");
	int dx=0, dy=0; // это переменные для прироста координат снаряда

	if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("\n mutex init failed\n");
        return 1;
    }// объявили мьютекс. взято из статьи
    
    // инициализируем ncurses
    initscr();
    curs_set(0);
    noecho();
    keypad(stdscr,TRUE);

    // инициализируем цвет
	center_x=COLS/2;// ЦЕНТР ИГРОВОГО ПОЛЯ
	center_y=LINES/2;
	start_color();// Это для того чтобы работать с цветами
	init_pair(1, COLOR_GREEN,COLOR_BLACK); // зелёный
	init_pair(2, COLOR_RED,COLOR_BLACK);// красный

	// запускаем "принимающий сокет" он принимает пакеты и модифицирует состояние игры.
	pthread_t server_t;
	pthread_create(&server_t,NULL, udp_server,NULL);// запускаем функцию udp сервер в отдельном потоке

	// старт потока, занимающегося выстрелами
	pthread_t gun_thread;// то же самое с функцией gun
	pthread_create(&gun_thread, NULL,gun,NULL);

	// запускаем "передающий сокет"
	int ret,yes=1;

	sock=socket(AF_INET,SOCK_DGRAM,0);

	if (sock<0){
		perror("sock");
		return -1;
	}

	ret=setsockopt(sock,SOL_SOCKET,SO_BROADCAST,(char*)&yes,sizeof(yes)); // функция для того чтобы задать общие параметры сокета (широковещательная рассылка)
	if (ret==-1){
		perror("setsockopt");
		return 0;
	}

	addr_len=sizeof(struct sockaddr_in);
	memset((void*)&addr,0,addr_len);
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr=htonl(INADDR_BROADCAST);
	addr.sin_port=htons(6667);// для запуска сокета	

    
    // генерируем случайное положение своего танка
    srand(time(NULL));
	center_x += rand()%80-40;
	center_y += rand()%40-20;
						
	//fprintf(tlog, "center_x=%d, center_y=%d \n", center_x, center_y);
	//fflush(tlog);


    // передаём данные о начальном положении своего танка
	send_coords();

	// рисуем начальное состояние - оно должно быть на экране независимо от нажатияя клавиш
    dulo=0; dx=0; dy=-1;
	draw_all();

    // начинаем цикл опроса клавиатуры
    int button;
    while((button=getch())!='u'){
    
        switch (button){
            case KEY_UP:
                center_y --; 
				//vrag_y++;           
                break;

            case KEY_DOWN:
                center_y++;
                break;

            case KEY_RIGHT:
                center_x++;
                break;
            
            case KEY_LEFT:
                center_x--;
                break;

            case 'w':
                dulo=0; dx=0; dy=-1;
				break;

            case 'e':
                dulo=1; dx=1; dy=-1;
                break;

            case 'd':
                dulo=2;
                break;

            case 'c':
                dulo=3;
                break;

            case 'x':
                dulo=4; dx=0; dy=1;
                break;

            case 'z':
                dulo=5;
                break;

            case 'a':
                dulo=6;
                break;

            case 'q':
                dulo=7; dx=-1; dy=-1;
                break;

			case 's':
				if (gun_x==-1){
					gun_x = center_x;
					gun_y = center_y;
					fdx = dx; fdy = dy;
					gun_x += fdx; 
					gun_y += fdy; 
				}
				break;

            default:
                break;
        }
        
        if (center_x<COLS/2-40)
	        center_x=COLS/2+40;

        if (center_y<LINES/2-20)
	        center_y=LINES/2+20;

        if (center_x>COLS/2+40)
            center_x=COLS/2-40;
        
        if (center_y>LINES/2+20)
            center_y=LINES/2-20;

		send_coords();
		//draw_all();
        
		usleep(10000);
    }
        


}
