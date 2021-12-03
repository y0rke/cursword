#include <ncurses.h>
#include <clocale>
#include <swmgr.h>
#include <markupfiltmgr.h>
#include <memory>
#include <cstring>
#include <utilstr.h>
#include "session.h"
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
	wchar_t buf[buf_h][buf_w];

	SWMgr library{new MarkupFilterMgr{FMT_PLAIN}};
	SWModule *bible = library.getModule("LEB");
	bible->setKey("gen 1:1");

	init_displaybuffer((wchar_t*)buf, buf_h, buf_w, bible);

	//Display the buffer
	for(int i = 0; i < buf_h; i++){
		mvwprintw(win, i + 1, 1, "%ls", buf[i]);
	}

	wrefresh(win);

	getch();
}
