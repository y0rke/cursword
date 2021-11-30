#pragma once

struct catstat{
	int nread;
	int nwritten;
};

catstat fmt_strncat(char* dst, const char* src, int n);
//int wide_strlen(wchar_t* str);
