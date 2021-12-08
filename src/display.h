#pragma once
#include <cwchar>
#include <ncurses.h>

struct DisplayBuf{
	int top;
	int height;
	int width;
	wchar_t* data;
};

struct Display{
	int height;
	int width;
	WINDOW* win;
	DisplayBuf buf;
};

int init_stdscr(int* stdscr_h, int* stdscr_w);
