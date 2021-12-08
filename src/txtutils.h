#pragma once

struct catstat{
	int nread;
	int nwritten;
};

catstat fmt_strncat(wchar_t* dst, const wchar_t* src, int n);
bool if_eosentence(wchar_t ch);
int fmtd_strlen(wchar_t str[]);
