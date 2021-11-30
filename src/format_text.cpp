#include "format_text.h"
#include <cstring>
#include <cctype>

catstat fmt_strncat(char* dst, const char* src, int n){
	int dst_len = strlen(dst);
	int src_len = strlen(src);
	int dst_i = dst_len;
	int src_i = 0;
	int count = 0;

	while(count < n && src_i < src_len){
		if(isascii(src[src_i]) && src[src_i] != '\n'){
			dst[dst_i] = src[src_i];
			dst_i++;
			count++;
		}
		src_i++;
	}

	dst[dst_i] = '\0';

	return catstat{src_i, count};//number of characters from src read and written 
}
