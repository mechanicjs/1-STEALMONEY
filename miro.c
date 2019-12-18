//compile: cc miro.c -o miro -lcurses -lpthread

#include <stdio.h>
#include <unistd.h>
#include <termio.h>
#include <curses.h>
#include <stdlib.h>
#include <pthread.h>

#define ROW 15
#define COL 26

#define WALL 'X'
#define TREASURE '$'

#define TIMER_ROW 3
#define TIMER_COL 30

#define LIMIT 30
#define PENALTY 3

char miro[ROW][COL];
int limit;

void* show_timer(void* m);
void draw_map(char* map_name);
int play();

int main(){

	initscr();
	noecho();
	curs_set(0);

for(int i=0;i<3;i++){
	draw_map("map.txt");
	if(!play()){ //game over
		clear();
		addstr("GAME OVER");
		refresh();
		sleep(3);
		break;
	}
	else{ //treasure!
		printf(" < YEAH!!!!!\n");
		sleep(1);
		limit=0;
		clear();
		addstr("go to next stage");
		refresh();
		sleep(3);
	}
}

	endwin();
	return 0;
}

void* show_timer(void* m)
	{
		limit=LIMIT;
		for(limit;limit>0;limit--){
			move(TIMER_ROW,TIMER_COL);
			addch('0'+limit/10);
			addch('0'+limit%10);
			refresh();
			sleep(1);
		}
		return NULL;
	}

void draw_map(char* map_name){
		FILE*fp=fopen(map_name,"r");
		if(!fp){
			printf("FILE cannot be opened\n");
			exit(1);
		}

		clear();
		for(int i=0;i<ROW;i++){
			fgets(miro[i],COL,fp);
			move(i,0);
			addstr(miro[i]);
		}
		fclose(fp);
}

int play(){
	char input;
	int cur_row=0;
	int cur_col=1;
	char* star="O";
  char* blank=" ";
	pthread_t t1;

	move(cur_row,cur_col);
	addstr(star);
	refresh();

	pthread_create(&t1,NULL,show_timer,NULL);
	while((input=getch())!='q'){
		if(limit<=0)return 0; //game over

		if(input==27){
			getch();
			input=getch();
			switch(input){
				case 65://down
						if(miro[cur_row-1][cur_col]==WALL){
							limit-=PENALTY; //시간 깎기
							break;
						}
						move(cur_row,cur_col);
						addstr(blank);
						cur_row-=1;
						move(cur_row,cur_col);
						addstr(star);
						refresh();
						break;
				case 66://up
						if(miro[cur_row+1][cur_col]==WALL){
							limit-=PENALTY; //시간 깎기
							break;
						}
						move(cur_row,cur_col);
						addstr(blank);
						cur_row+=1;
						move(cur_row,cur_col);
						addstr(star);
						refresh();
						break;
				case 68: //left
						if(miro[cur_row][cur_col-1]==WALL) {
							limit-=PENALTY; //시간 깎기
							break;
						}
						move(cur_row,cur_col);
						addstr(blank);
						cur_col-=1;
						move(cur_row,cur_col);
						addstr(star);
						refresh();
						break;
				case 67: //right
					if(miro[cur_row][cur_col+1]==WALL) {
						limit-=PENALTY; //시간 깎기
						break;
					}
					move(cur_row,cur_col);
					addstr(blank);
					cur_col+=1;
					move(cur_row,cur_col);
					addstr(star);
					refresh();
			}
			if(miro[cur_row][cur_col]==TREASURE) return 1;
		}
	}
}
