#pragma once

struct catstat{
	int nread;
	int nwritten;
};

catstat fmt_strncat(wchar_t* dst, const wchar_t* src, int n);
