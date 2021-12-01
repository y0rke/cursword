#include "format_text.h"
#include <cstring>
#include <cwctype>
#include <cwchar>

catstat fmt_strncat(wchar_t* dst, const wchar_t* src, int n){
	int dst_len = wcslen(dst);
	int src_len = wcslen(src);
	int dst_i = dst_len;
	int src_i = 0;
	int count = 0;

	while(count < n && src_i < src_len){
		if(iswprint(src[src_i])){
			dst[dst_i] = src[src_i];
			dst_i++;
			count++;
		}
		src_i++;
	}

	dst[dst_i] = L'\0';

	return catstat{src_i, count};//number of characters from src read and written 
}
