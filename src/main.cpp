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
	init_bibdp(&bibdp, stdscr_h, stdscr_w);
	//end

	SWMgr library{new MarkupFilterMgr{FMT_PLAIN}};
	SWModule *bible = library.getModule("LEB");

	int err = set_bibdpbuf_to_verse_context(&bibdp.buf, bible, "gen 1:1");
	if(err != 0) exit(EXIT_FAILURE);

	bool is_running = true;
	while(is_running){
		//Display the buffer
		present_bible_dp(&bibdp);

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
