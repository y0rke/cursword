#include <ncurses.h>
#include <clocale>
#include <swmgr.h>
#include <markupfiltmgr.h>
#include <memory>
#include <cstring>
#include <utilstr.h>
#include <versekey.h>
#include <cstdlib>
#include "format_text.h"
#include "display.h"
#include "bible.h"

using namespace sword;

int main(int argc, char* argv[]){
	//ncurses init
	setlocale(LC_ALL, "");
	initscr();
	start_color();
	cbreak();
	clear();
	noecho();
	curs_set(0);
	refresh();
	int stdscr_h, stdscr_w;
	getmaxyx(stdscr, stdscr_h, stdscr_w);//not zero based
	//end

	//init display
	BibleDisplay bibdp{};
	bibdp.height = stdscr_h; 
	bibdp.width = stdscr_w / 2;
	bibdp.win = newwin(bibdp.height, bibdp.width, 0, bibdp.width - bibdp.width/2);
	keypad(bibdp.win, TRUE);
	box(bibdp.win, 0, 0);
	bibdp.buf.height = bibdp.height - 2;//Account for box
	bibdp.buf.width = bibdp.width - 1;//- 1 to leave space for the null byte
	bibdp.buf.top = 0;
	bibdp.buf.data = (wchar_t*)calloc(bibdp.buf.height * bibdp.buf.width, sizeof(wchar_t));
	//end

	SWMgr library{new MarkupFilterMgr{FMT_PLAIN}};
	SWModule *bible = library.getModule("LEB");
	bible->setKey("gen 1:1");

	write_to_bible_buf(&bibdp.buf, bible);
	
	bool is_running = true;
	while(is_running){
		//Display the buffer
		for(int i = 0; i < bibdp.buf.height; i++){
			mvwprintw(bibdp.win, i + 1, 1, "%ls", (bibdp.buf.data + i * bibdp.buf.width));
		}

		wrefresh(bibdp.win);

		switch(getch()){
			case 'q':
				is_running = false;
				break;
			case 'k':
			case 'j':
			default: is_running = false;
		}
	}

	free(bibdp.buf.data);
}
