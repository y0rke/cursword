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

	int height, width;
	getmaxyx(stdscr, height, width);

	SWMgr library(new MarkupFilterMgr(FMT_PLAIN));

	SWModule *target = library.getModule("LEB");
	target->setKey("gen 1:1");
	target->renderText();

	printw("%s\n %s", target->getKeyText(), (const char*)target->renderText());

	getch();
}
