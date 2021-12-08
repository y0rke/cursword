#include <ncurses.h>
#include <clocale>
#include <swmgr.h>
#include <markupfiltmgr.h>
#include <memory>
#include <cstring>
#include <utilstr.h>
#include <versekey.h>
#include <cstdlib>
#include "display.h"
#include "bible.h"

using namespace sword;

int main(int argc, char* argv[]){
	std::string txt_key = "gen 1:1";
	int line = 0;
	if(argc >= 2){
		txt_key = argv[1];
	}
	if(argc == 3){
		line = atoi(argv[2]);
	}

	//ncurses init
	int stdscr_h, stdscr_w;
	init_stdscr(&stdscr_h, &stdscr_w);
	//end

	//init display
	BibleDisplay bibdp{};
	init_bibdp(&bibdp, stdscr_h, stdscr_w/2, 0, stdscr_w/4);
	//end

	SWMgr library{new MarkupFilterMgr{FMT_PLAIN}};
	SWModule *bible = library.getModule("LEB");

	int err = set_bibdpbuf_to_verse_context(&bibdp.buf, bible, txt_key, line);
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
