#include "session.h"
#include <cwchar>
#include "format_text.h"

void init_displaybuffer(wchar_t* buf, int buf_h, int buf_w, SWModule* module){
	for(int i = 0; i < buf_h; i++){
		*(buf + i * buf_w) = L'\0';
	}

	int line = 0;
	int verse_i = 0;
	int space_left; 
	int verse_len;
	catstat stats;
	wchar_t* verse;

	SWBuf verse_buf = utf8ToWChar(module->renderText());

	//Write to the display buffer
	while(line < buf_h){
		verse = (wchar_t*)(verse_buf.getRawData());
		verse_len = wcslen(&verse[verse_i]);

		space_left = buf_w - wcslen(buf + line * buf_w) - 1;//- 1 reserves a space for null byte
		stats = fmt_strncat(buf + line * buf_w, &verse[verse_i], space_left);

		if(stats.nwritten < space_left && stats.nread == verse_len){
			if(if_eosentence(verse[verse_i + verse_len - 1]) && (space_left - stats.nwritten) > 2){
				wcscat(buf + line * buf_w, L"  ");//Add double space
			}
			else if(verse[verse_i + verse_len - 1] == L'\n'){
				line++;
			}

			(*module)++;
			verse_buf = utf8ToWChar(module->renderText());
			verse_i = 0;
		}
		else if(stats.nwritten == space_left && stats.nread < verse_len){
			verse_i += stats.nread;
			line++;
		}
		else{
			verse_i = 0;
			line++;
			(*module)++;
			verse_buf = utf8ToWChar(module->renderText());
		}
	}
}
