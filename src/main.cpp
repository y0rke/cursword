#include <ncurses.h>
#include <clocale>
#include <swmgr.h>
#include <markupfiltmgr.h>
#include <memory>
#include <cstring>
#include "format_text.h"
#include <cstdio>

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

	for(int i = 0; i < buf_h; i++){
		buf[i][0] = 0;
	}

	SWMgr library{new MarkupFilterMgr{FMT_PLAIN}};
	SWModule *leb = library.getModule("LEB");
	leb->setKey("gen 1:1");

	int line = 0;
	int verse_i = 0;
	int space_left; 
	int verse_len;
	catstat stats;

	while(line < buf_h){
		const char* verse = leb->stripText();
		space_left = buf_w - strlen(buf[line]) - 1;//- 1 reserves a space for null byte

		stats = fmt_strncat(buf[line], &verse[verse_i], space_left);

		verse_len = strlen(&verse[verse_i]);
		if(stats.nwritten < space_left && stats.nread < verse_len){
			verse_i += stats.nread;
		}
		else if(stats.nwritten < space_left && stats.nread == verse_len){
			verse_i = 0;
			(*leb)++;
		}
		else if(stats.nwritten == space_left && stats.nread < verse_len){
			verse_i += stats.nread;
			line++;
		}
		else{
			verse_i = 0;
			line++;
			(*leb)++;
		}
	}

	for(int i = 0; i < buf_h; i++){
		mvwprintw(win, i + 1, 1, "%s", buf[i]);
	}
	wrefresh(win);

	getch();
}
