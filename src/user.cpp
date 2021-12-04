#include "user.h"
#include <versekey.h>

void scroll_down(display_win* buf, SWModule* module, int lines){
	if(lines > 0){
		int startline = buf->top;
		buf->top += lines;
		if(buf->top >= buf->height){
			buf->top = buf->height - buf->top;
		}

		if(startline < buf->top){
			VerseKey end_verse{};
			end_verse.setBook(buf->ends.last_key.book);
			end_verse.setChapter(buf->ends.last_key.chapter);
			end_verse.setVerse(buf->ends.last_key.verse);
			module->setKey(end_verse);

			int line = startline;

			while(line < buf->top){
			}
		}
		else{
		}
	}
}

void scroll_up(display_win* buf, SWModule* module, int lines){
}
