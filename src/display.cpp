#include "display.h"
#include <clocale>

int init_stdscr(int* stdscr_h, int* stdscr_w){
	setlocale(LC_ALL, "");
	initscr();
	start_color();
	cbreak();
	clear();
	noecho();
	curs_set(0);
	refresh();	
	getmaxyx(stdscr, *stdscr_h, *stdscr_w);//not zero based
	return 0;
}

