#include "bible.h"
#include <string>
#include "format_text.h"

void write_to_bible_dpbuf(DisplayBuf* buf, SWModule* bib){
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

Verse* get_verse_buffer(SWModule* bib, std::string key = "gen 1:1"){
	return nullptr;
}

void present_bible_dp(BibleDisplay* bibdp){
	for(int i = 0; i < bibdp->buf.height; i++){
		mvwprintw(bibdp->win, i + 1, 1, "%ls", (bibdp->buf.data + i * bibdp->buf.width));
	}

	wrefresh(bibdp->win);
}

int set_bibdpbuf_to_verse_context(DisplayBuf* dpbuf, SWModule* bib, std::string txtKey, int lineNo){
	int err = bib->setKey(txtKey.c_str());
	if(err != 0) return err;
	if(lineNo < 0 || lineNo >= dpbuf->height) return 256;//2^sizeof(char)

	int topLine = 0;
	int bottomLine = dpbuf->height - 1;

	SWBuf verse = utf8ToWChar(bib->renderText());
	wchar_t (*buf)[dpbuf->width] = (wchar_t (*)[dpbuf->width]) dpbuf->data;//Arcane, but useful

	for(int i = 0; i < dpbuf->height; i++){
		buf[i][0] = 0;
	}

	int space_left = dpbuf->width - wcslen(buf[lineNo]) - 1;

	wcsncat(buf[lineNo], (wchar_t*)verse.getRawData(), space_left);

	return 0;
};

int init_bibdp(BibleDisplay* bibdp, int stdscr_h, int stdscr_w){
	bibdp->height = stdscr_h; 
	bibdp->width = stdscr_w / 2;
	bibdp->win = newwin(bibdp->height, bibdp->width, 0, bibdp->width - bibdp->width/2);

	if(bibdp->win == NULL) return -1;

	keypad(bibdp->win, TRUE);
	box(bibdp->win, 0, 0);
	bibdp->buf.height = bibdp->height - 2;//Account for box
	bibdp->buf.width = bibdp->width - 1;//- 1 to leave space for the null byte
	bibdp->buf.top = 0;
	bibdp->buf.data = (wchar_t*)calloc(bibdp->buf.height * bibdp->buf.width, sizeof(wchar_t));

	return 0;
}
