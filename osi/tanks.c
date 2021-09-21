#include <ncurses.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#define PORT 9997
//int flag_shoot;

typedef struct {
    int x;
    int y;
} coordinats;


typedef struct host {
    struct host *prev;
    struct host *next;

    in_addr_t ip;
    coordinats location;
    int addiction;
    int actual;
} host;

typedef struct {
    struct host *first;
} listing;


typedef struct shot {
    struct shot *next;

    coordinats pos;
    coordinats vel;
} shot;

typedef struct {
    shot *first;
} shot_listing;

int gamers = 2;

int server_running;
int actual_players = 0;

listing players = {NULL};
shot_listing shots = {NULL};

void get_tank(coordinats *location, int c) {
    switch (c) {
        case 1:
            location->x = 9;
            location->y = LINES/2 - 1;
            break;
        case 2:
            location->x = COLS - 10;
            location->y = LINES/2 - 1;
            break;
        default:
            break;
    }
}

void shoot(int x_from, int y_from, int vel_x, int vel_y) {
    shot *new_shot = (shot *) malloc(sizeof(shot));
    new_shot->next = NULL;
    new_shot->pos.x = x_from;
    new_shot->vel.x = vel_x;
    new_shot->pos.y = y_from;
    new_shot->vel.y = vel_y;



    if (shots.first == NULL) {
        shots.first = new_shot;
        return;
    }
    shot *bullet = shots.first;
    for (; bullet->next != NULL; bullet = bullet->next);
    bullet->next = new_shot;
}

void change_shot(shot *cur) {
    if (shots.first == NULL)
        return;

    if (shots.first == cur) {
        if (cur->next != NULL)
            shots.first = cur->next;
        else {
            shots.first = NULL;
        }
        return;
    }

    shot *prev = shots.first;
    for (; prev->next != cur && prev->next != NULL; prev = prev->next);

    if (prev->next != NULL)
        prev->next = cur->next;
}

host *add_new_back(listing *list, in_addr_t ip) {
    host *last = list->first;
    if (last != NULL)
        for (; last->next != NULL; last = last->next);

    host *new_listing = (host *) malloc(sizeof(host));
    new_listing->ip = ip;

    actual_players += 1;
    get_tank(&(new_listing->location), actual_players);

    new_listing->addiction = 0;
    new_listing->actual = 2;

    if (list->first) {
        new_listing->prev = last;
        new_listing->next = NULL;
        last->next = new_listing;
    } else {
        list->first = new_listing;
        new_listing->prev = NULL;
        new_listing->next = NULL;
    }
    return new_listing;
}

host *get_LIST(listing *list, in_addr_t ip) {
    host *ret = NULL;
    for (host *node = list->first; node != NULL; node = node->next)
        if (node->ip == ip)
            return node;
    return ret;
}

void make_first(listing *list, host *node) {
    if (list->first == NULL || list->first == node)
        return;

    host *prev = list->first;
    for (; prev->next != node && prev->next != NULL; prev = prev->next);

    if (prev->next != NULL)
        prev->next = node->next;

    node->next = list->first;
    list->first = node;
}

void input(host *player, int ch) {
    switch (ch) {
        case 'q':
            player->actual = 1;
            break;
        case 'w':
            player->actual = 2;
            break;
        case 'e':
            player->actual = 3;
            break;
        case 'd':
            player->actual = 4;
            break;
        case 'c':
            player->actual = 5;
            break;
        case 'x':
            player->actual = 6;
            break;
        case 'z':
            player->actual = 7;
            break;
        case 'a':
            player->actual = 8;
            break;
        default:
            break;
    }

    if (ch == 's') {
        int shoot_pos_x, shoot_pos_y;
        int shoot_vel_x, shoot_vel_y;
//	if (flag_shoot)
//	{
        	switch (player->actual) {
           	case 1:
               		shoot_pos_x = -1;
               		shoot_pos_y = -1;
                	shoot_vel_x = -1;
                	shoot_vel_y = -1;
                	break;
            	case 2:
                	shoot_pos_x = 1;
                	shoot_pos_y = -1;
                	shoot_vel_x = 0;
                	shoot_vel_y = -1;
                	break;
           	case 3:
                	shoot_pos_x = 3;
                	shoot_pos_y = -1;
                	shoot_vel_x = 1;
                	shoot_vel_y = -1;
                	break;
            	case 4:
                	shoot_pos_x = 3;
                	shoot_pos_y = 1;
                	shoot_vel_x = 1;
                	shoot_vel_y = 0;
                	break;
            	case 5:
                	shoot_pos_x = 3;
                	shoot_pos_y = 3;
                	shoot_vel_x = 1;
                	shoot_vel_y = 1;
                	break;
            	case 6:
               		shoot_pos_x = 1;
                	shoot_pos_y = 3;
                	shoot_vel_x = 0;
                	shoot_vel_y = 1;
                	break;
            	case 7:
                	shoot_pos_x = -1;
                	shoot_pos_y = 3;
                	shoot_vel_x = -1;
                	shoot_vel_y = 1;
                	break;
            	case 8:
                	shoot_pos_x = -1;
                	shoot_pos_y = 1;
                	shoot_vel_x = -1;
                	shoot_vel_y = 0;
                	break;
            	default:
                	break;
       		}
//		flag_shoot = 0;
        	shoot(player->location.x + shoot_pos_x, player->location.y + shoot_pos_y, shoot_vel_x, shoot_vel_y);
//	}
    }

    if (ch == KEY_UP) {
        player->addiction = 0;

        player->location.y -= 1;
        if (player->location.y < 0)
            player->location.y = 0;
    }
    if (ch == KEY_DOWN) {
        player->addiction = 0;

        player->location.y += 1;
        if (player->location.y > LINES - 3)
            player->location.y = LINES - 3;
    }
    if (ch == KEY_RIGHT) {
        player->addiction = 1;

        player->location.x += 1;
        if (player->location.x > COLS - 3)
            player->location.x = COLS - 3;
    }
    if (ch == KEY_LEFT) {
        player->addiction = 1;

        player->location.x -= 1;
        if (player->location.x < 0)
            player->location.x = 0;
    }
}

void draw(char *tank, coordinats pos) {
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            mvaddch(pos.y + y, pos.x + x, tank[x + 3 * y]);
        }
    }
}

void make(char *tank, int dir, int addiction) {
    for (int i = 0; i < 9; i++) {
        tank[i] = '#';
    }
    if (dir == 0) {
        tank[1] = ' ';
        tank[7] = ' ';
    } else {
        tank[3] = ' ';
        tank[5] = ' ';
    }

    switch (addiction) {
        case 1:
            tank[0] = '\\';
            break;
        case 2:
            tank[1] = '|';
            break;
        case 3:
            tank[2] = '/';
            break;
        case 4:
            tank[5] = '-';
            break;
        case 5:
            tank[8] = '\\';
            break;
        case 6:
            tank[7] = '|';
            break;
        case 7:
            tank[6] = '/';
            break;
        case 8:
            tank[3] = '-';
            break;

        default:
            break;
    }

}


pthread_mutex_t mutex;

int Win = 0;

void *udp_server(void *none) {
    int sock, addr_len, count, ret;
    fd_set readfd;
    struct sockaddr_in addr;

    int started = 0;

    int opt = 3;
    int col;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (0 > sock) {
        perror("socket");
        return NULL;
    }

    setsockopt(sock, SOL_SOCKET, SO_RCVLOWAT, &opt, sizeof(opt));

    addr_len = sizeof(struct sockaddr_in);
    memset((void *) &addr, 0, addr_len);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htons(INADDR_ANY);
    addr.sin_port = htons(PORT);

    if (bind(sock, (struct sockaddr *) &addr, addr_len) < 0) {
        perror("bind");
        close(sock);
        return NULL;
    }

    pthread_mutex_lock(&mutex);
    for (host *node = players.first; node != NULL; node = node->next) {
        char tank[9];
        make(tank, node->addiction, node->actual);
	if (node->next != NULL)
		col = 1;
	else
		col = 2;
	attron(COLOR_PAIR(col));
        draw(tank, node->location);
	attroff(COLOR_PAIR(col));
    }

    pthread_mutex_unlock(&mutex);

    while (server_running!=0) {
        struct timeval timeoff;
        timeoff.tv_sec = 0;
        timeoff.tv_usec = 10000;

        FD_ZERO(&readfd);
        FD_SET(sock, &readfd);

        int ch;
        ret = select(sock + 1, &readfd, NULL, NULL, &timeoff);
        if (ret > 0) {
            if (FD_ISSET(sock, &readfd)) {
                count = recvfrom(sock, &ch, sizeof(ch), 0, (struct sockaddr *) &addr, &addr_len);
                host *player;
                if (player = get_LIST(&players, addr.sin_addr.s_addr)) {
                    if (started != 1) {
                        make_first(&players, player);

                        actual_players = 0;

                        for (host *node = players.first; node != NULL; node = node->next) {
                            actual_players += 1;
                            get_tank(&(node->location), actual_players);
                        }

                        started = 1;
                    }

                    if (started == 1)
                        input(player, ch);
                }
            }
        }

        if (actual_players >= gamers && started) {
            pthread_mutex_lock(&mutex);
            clear();
            for (shot *bullet = shots.first; bullet != NULL; bullet = bullet->next) {
                coordinats *bulletinside = &(bullet->pos);
                bulletinside->x += bullet->vel.x;
                bulletinside->y += bullet->vel.y;

                if (bulletinside->x < 0 || bulletinside->y < 0 || bulletinside->x > COLS || bulletinside->y > LINES) {
                    change_shot(bullet);
//		    flag_shoot = 1;
                    continue;
                }

                mvaddch(bulletinside->y, bulletinside->x, '*');
            }

            int currentTank = 1;
            for (host *node = players.first; node != NULL; node = node->next) {
                char tank[9];
                make(tank, node->addiction, node->actual);
		if (node->next != NULL)
			col = 1;
		else
			col = 2;
		attron(COLOR_PAIR(col));
                draw(tank, node->location);
		attroff(COLOR_PAIR(col));


                for (shot *bullet = shots.first; bullet != NULL; bullet = bullet->next) {
                    coordinats *bulletinspace = &(bullet->pos);
                    if (bulletinspace->x >= node->location.x && bulletinspace->x <= node->location.x + 2 &&
                        bulletinspace->y >= node->location.y && bulletinspace->y <= node->location.y + 2) {
                        Win = currentTank;
			usleep(1000000);
                        break;
                    }
                }

                if (Win != 0) {
//			pthread_mutex_lock(&mutex);
                    server_running = 0;
		    pthread_mutex_unlock(&mutex);
    //                fprintf(stderr, "%s:%d\twin%d\t%d\n", __func__, __LINE__, Win, server_running);
		    return (void *) NULL;
                }

                currentTank += 1;
            }
            refresh();
            pthread_mutex_unlock(&mutex);
        }
    }
    close(sock);
    return (void *) NULL;
}

int get_input() {
    pthread_mutex_lock(&mutex);
    int a = getch();
    pthread_mutex_unlock(&mutex);
    usleep(100);
    return a;
}

int main(int argc, char **argv) {
    int sock, yes = 1;
    pthread_t pid;
    struct sockaddr_in addr;
    int addr_len;
    int ret = 0;

  //  flag_shoot = 1;
    players.first = NULL;

    pthread_mutex_init(&mutex, 0);

    if (argc != 3) {
        printf("Usage: %s ip1 ip2\n", argv[0]);
        return -1;
    }

    server_running = 1;
    initscr();

    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    timeout(1);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);

    add_new_back(&players, inet_addr(argv[1]));
    add_new_back(&players, inet_addr(argv[2]));

    pthread_create(&pid, NULL, udp_server, NULL);
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("sock");
        return -1;
    }
    ret = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *) &yes, sizeof(yes));
    if (ret == -1) {
        perror("setsockopt");
        return 0;
    }

    addr_len = sizeof(struct sockaddr_in);
    memset((void *) &addr, 0, addr_len);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    addr.sin_port = htons(PORT);

    int ch;

    while (server_running != 0 && (ch = get_input()) != 'i') {
        if (ch > 0) {
//		fprintf(stderr, "%s:%d\tserver%d\n", __func__, __LINE__, server_running);
            sendto(sock, &ch, sizeof(ch), 0, (struct sockaddr *) &addr, addr_len);
        }
    }
  //  fprintf(stderr, "%s:%d\tposlwhile%d\n", __func__, __LINE__, server_running);
 //   server_running = 0;
    clear();
    refresh();
    pthread_join(pid, NULL);
    endwin();
    pthread_mutex_destroy(&mutex);

    close(sock);

    switch (Win) {
        case 1:
            mvaddstr(LINES/2, COLS/2,"PLAYER 2 wins\n");
            break;
        case 2:
            mvaddstr(LINES/2, COLS/2,"PLAYER 1 wins\n");
            break;
    }
    refresh();
    usleep(3000000);
/*    server_running = 0;
    refresh();
    usleep(3000000);
    pthread_join(pid, NULL);
    endwin();
    pthread_mutex_destroy(&mutex);
    close(sock);
*/
    return 0;
}

