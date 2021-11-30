#include <ncurses.h>
#include <clocale>
#include <swmgr.h>
#include <markupfiltmgr.h>
#include <memory>
#include <cstring>
#include "format_text.h"

using namespace sword;

int main(int argc, char* argv[]){
	//ncurses init
	setlocale(LC_ALL, "");
	initscr();
	start_color();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	refresh();
	//end

	int stdscr_h, stdscr_w;
	getmaxyx(stdscr, stdscr_h, stdscr_w);//not zero based

	int win_h = stdscr_h;
	int win_w = stdscr_w / 2;
	WINDOW *win = newwin(win_h, win_w, 0, win_w - win_w/2);
	box(win, 0, 0);

	//Account for box
	int buf_h = win_h - 2;
	int buf_w = win_w - 1;
	char buf[buf_h][buf_w];

	/*
	for(int i = 0; i < buf_h; i++){
		for(int j = 0; j < buf_w; j++){
			buf[i][j] = 'X';
			if(j == buf_w - 1) buf[i][j] = 0;
		}
	}
	*/
	for(int i = 0; i < buf_h; i++){
		buf[i][0] = 0;
	}

	SWMgr library{new MarkupFilterMgr{FMT_PLAIN}};
	SWModule *leb = library.getModule("LEB");
	leb->setKey("gen 1:1");

	for(int i = 0; i < buf_h; i++){
		fmt_strncat(buf[i], leb->stripText(), buf_w - 1);
		(*leb)++;
	}

	for(int i = 0; i < buf_h; i++){
		mvwprintw(win, i + 1, 1, "%s", buf[i]);
	}
	wrefresh(win);

	getch();
}
