#include "bible.h"
#include "format_text.h"

void write_to_bible_buf(DisplayBuf* buf, SWModule* bib){
	for(int i = 0; i < buf->height; i++){
		*(buf->data + i * buf->width) = L'\0';
	}

	int line = 0;
	int verse_i = 0;
	int space_left; 
	int verse_len;
	catstat stats;
	wchar_t* verse;

	SWBuf verse_buf = utf8ToWChar(bib->renderText());

	//Write to the display buffer
	while(line < buf->height){
		verse = (wchar_t*)(verse_buf.getRawData());
		verse_len = wcslen(&verse[verse_i]);

		space_left = buf->width - wcslen(buf->data + line * buf->width) - 1;//- 1 reserves a space for null byte
		stats = fmt_strncat(buf->data + line * buf->width, &verse[verse_i], space_left);

		if(stats.nwritten < space_left && stats.nread == verse_len){
			if(if_eosentence(verse[verse_i + verse_len - 1]) && (space_left - stats.nwritten) > 2){
				wcscat(buf->data + line * buf->width, L"  ");//Add double space
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
}
