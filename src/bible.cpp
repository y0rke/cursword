#include "bible.h"
#include <string>
#include "txtutils.h"

static const std::string gen_1_1 = "gen 1:1";
static const std::string rev_22_21 = "rev 22:21";

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
	VerseKey verse_key{txtKey.c_str()};
	/*
	verse_key.setLowerBound(gen_1_1.c_str());
	verse_key.setUpperBound(rev_22_21.c_str());
	VerseKey lowerBound = verse_key.getLowerBound();
	VerseKey upperBound = verse_key.getUpperBound();
	*/

	int err = bib->setKey(verse_key);
	if(err != 0) return err;
	if(lineNo < 0 || lineNo >= dpbuf->height) return -1;

	int top_line = 0;
	int bottom_line = dpbuf->height - 1;

	wchar_t (*buf)[dpbuf->width] = (wchar_t (*)[dpbuf->width]) dpbuf->data;//Arcane, but useful

	for(int i = 0; i < dpbuf->height; i++){
		buf[i][0] = L'\0';//Clear buffer
	}

	SWBuf verse_buf = utf8ToWChar(bib->renderText());
	wchar_t* verse = (wchar_t*)verse_buf.getRawData();
	int verse_index = 0;
	int verse_len = wcslen(verse);
	int space = dpbuf->width - 1;
	catstat stat;
	int up_line = lineNo;//Traverses up the buffer
	int down_line = lineNo;//Traverses down the buffer

	do{
		stat = fmt_strncat(buf[down_line], &verse[verse_index], space);
		verse_index += stat.nread;
		if(stat.nwritten == space && verse_index < verse_len){
			down_line++;
		}
	}while(verse_index < verse_len);
	verse_index = 0;

	up_line--;
	down_line++;

	while(up_line >= top_line){//&& !lowerBound.equals(*((*bib)--).getKey())){
		(*bib)--;
		verse_buf = utf8ToWChar(bib->renderText());
		verse = (wchar_t*)verse_buf.getRawData();
		verse_len = fmtd_strlen(verse);

		if(verse_len > space){
			int remainder = verse_len % space;
			verse_index = verse_len - remainder;

			while(up_line >= top_line && verse_index >= 0){
				stat = fmt_strncat(buf[up_line], &verse[verse_index], space);
				up_line--;
				verse_index -= space;
			}

			verse_index = 0;
		}
		else{
			stat = fmt_strncat(buf[up_line], verse, space);
		}

		up_line--;
	}

	return 0;
};

int init_bibdp(BibleDisplay* bibdp, int height, int width, int start_y, int start_x){
	bibdp->height = height; 
	bibdp->width = width;
	bibdp->win = newwin(bibdp->height, bibdp->width, start_y, start_x);

	if(bibdp->win == NULL) return -1;

	keypad(bibdp->win, TRUE);
	box(bibdp->win, 0, 0);
	bibdp->buf.height = bibdp->height - 2;//Account for box
	bibdp->buf.width = bibdp->width - 1;//- 1 to leave space for the null byte
	bibdp->buf.top = 0;
	bibdp->buf.data = (wchar_t*)calloc(bibdp->buf.height * bibdp->buf.width, sizeof(wchar_t));

	return 0;
}
