#include "bible.h"
#include "format_text.h"

void write_to_bible_buf(DisplayBuf* buf, SWModule* bib){
	wchar_t stack_buf[buf->height][buf->width];
	for(int i = 0; i < buf->height; i++){
		stack_buf[i][0] = L'\0';
	}

	int line = 0;
	int verse_i = 0;
	int space_left; 
	int verse_len;
	catstat stats;
	wchar_t* verse;

	SWBuf verse_buf = utf8ToWChar(bib->renderText());

	//Write to the stack buffer
	while(line < buf->height){
		verse = (wchar_t*)(verse_buf.getRawData());
		verse_len = wcslen(&verse[verse_i]);

		space_left = buf->width - wcslen(stack_buf[line]) - 1;//- 1 reserves a space for null byte
		stats = fmt_strncat(stack_buf[line], &verse[verse_i], space_left);

		if(stats.nwritten < space_left && stats.nread == verse_len){
			if(if_eosentence(verse[verse_i + verse_len - 1]) && (space_left - stats.nwritten) > 2){
				wcscat(stack_buf[line], L"  ");//Add double space
			}
			else if(verse[verse_i + verse_len - 1] == L'\n'){
				line++;
			}

			(*bib)++;
			verse_buf = utf8ToWChar(bib->renderText());
			verse_i = 0;
		}
		else if(stats.nwritten == space_left && stats.nread < verse_len){
			verse_i += stats.nread;
			line++;
		}
		else{
			verse_i = 0;
			line++;
			(*bib)++;
			verse_buf = utf8ToWChar(bib->renderText());
		}
	}

	//Flush the stack buffer to the display buffer
	for(int i = 0; i < buf->height; i++){
		for(int j = 0; j < buf->width; j++){
			*((buf->data + i * buf->width) + j) = stack_buf[i][j];
		}
	}
}
