#include <stdio.h>
#include <unistd.h>
#include <termio.h>
#include <curses.h>
#include <stdlib.h>

#define ROW 15
#define COL 26

#define WALL 'X'
#define TREASURE '$'

int main(){
	char input;

	initscr();
	clear();
	noecho();
	curs_set(0);

	int cur_row=0;
	int cur_col=1;
	char* star="O";
  char* blank=" ";


	FILE*fp=fopen("map.txt","r");
	if(!fp){
		printf("FILE cannot be opened\n");
		exit(1);
	}

	char miro[ROW][COL];
	for(int i=0;i<ROW;i++){
		fgets(miro[i],COL,fp);
		move(i,0);
		addstr(miro[i]);
	}
	fclose(fp);


	move(cur_row,cur_col);
	addstr(star);
	refresh();

	while((input=getch())!='q'){
		if(input==27){
			getch();
			input=getch();
			switch(input){
				case 65://down
						if(miro[cur_row-1][cur_col]==WALL) break;
						move(cur_row,cur_col);
						addstr(blank);
						cur_row-=1;
						move(cur_row,cur_col);
						addstr(star);
						refresh();
						break;
				case 66://up
						if(miro[cur_row+1][cur_col]==WALL) break;
						move(cur_row,cur_col);
						addstr(blank);
						cur_row+=1;
						move(cur_row,cur_col);
						addstr(star);
						refresh();
						break;
				case 68: //left
						if(miro[cur_row][cur_col-1]==WALL) break;
						move(cur_row,cur_col);
						addstr(blank);
						cur_col-=1;
						move(cur_row,cur_col);
						addstr(star);
						refresh();
						break;
				case 67: //right
					if(miro[cur_row][cur_col+1]==WALL) break;
					move(cur_row,cur_col);
					addstr(blank);
					cur_col+=1;
					move(cur_row,cur_col);
					addstr(star);
					refresh();
			}
			if(miro[cur_row][cur_col]==TREASURE){
				printf(" < YEAH!!!!!\n");
			}
		}
	}

	endwin();
	return 0;
}
