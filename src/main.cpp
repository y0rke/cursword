#include <cstdio>
#include <ncurses.h>
#include <clocale>
#include <swmgr.h>
#include <markupfiltmgr.h>

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

	SWMgr library(new MarkupFilterMgr(FMT_PLAIN));
	ModMap::iterator it;
	for (it = library.Modules.begin(); it != library.Modules.end(); it++) {
		printw("[%s]\t - %s\n", (*it).second->getName(), (*it).second->getDescription());
	}

	getch();
}
