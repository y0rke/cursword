#include "format_str.h"
#include <cstring>
#include <cstdio>

int fmt_strncat(char* dst, const char* src, int n){
	int dst_len = strlen(dst);
	int src_len = strlen(src);
	int dst_i = dst_len;
	int src_i = 0;
	int count = 0;

	while(dst_i - dst_len <= n && src_i < src_len){
		if(src[src_i] != '\n'){
			dst[dst_i] = src[src_i];
			dst_i++;
			count++;
		}
		src_i++;
	}

	dst[dst_i] = '\0';

	return count;
}
