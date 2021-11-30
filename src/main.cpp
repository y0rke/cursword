#include <cstdio>
#include <ncurses.h>
#include <clocale>
#include <swmgr.h>
#include <markupfiltmgr.h>
#include <versekey.h>
#include <memory>
#include <cstring>
#include "format_str.h"

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

	int height, width;
	getmaxyx(stdscr, height, width);//not zero based

	int view_h = height;
	int view_w = width + 1;//+1 accounts for null byte in string
	char view[view_h][view_w];
	int top_line = 0;

	for(int i = 0; i < view_h; i++){
		view[i][0] = '\0';//init strings of size 0
	}

	SWMgr library{};
	SWModule *leb = library.getModule("LEB");
	leb->setKey("gen 1:1");

	int line = 0;
	int max_space = view_w - 1;//as prescribed in man strcat - leave room for null byte
	int verse_index = 0;

	while(line < view_h){
		const char* verse = leb->stripText();

		int line_space = max_space - strlen(view[line]);
		int verse_len = strlen(&verse[verse_index]);
		fmt_strncat(view[line], &verse[verse_index], line_space);

		if(verse_len < line_space){
			(*leb)++;
			verse_index = 0;
		}
		else{
			line++;
			verse_index += verse_len - (verse_len - line_space);
		}
	}

	for(int i = 0; i < view_h; i++){
		printw("%s", view[i]);
	}
/*
	const char* verse = leb->stripText();
	int verse_len = strlen(verse);

	char line[strlen(verse)];

	int line_len = 0;
	for(int i = 0; i < verse_len; i++){
		if(verse[i] != '\n'){
			line[line_len] = verse[i];
			line_len++;
		}
	}

	printw("%s", line);
	*/
	getch();
}
