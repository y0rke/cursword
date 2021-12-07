#pragma once
#include "display.h"
#include <swmodule.h>
#include <versekey.h>

using namespace sword;

struct BibKey{
	char book;
	int chapter;
	int verse;
};

struct KeyBounds{
	struct : BibKey{
		int start_index;
	} begin_key;

	struct : BibKey{
		int end_index;
	} end_key;
};

struct BibleDisplay : Display{
	KeyBounds bounds;
};

struct Verse{
	VerseKey key;
	SWBuf text;
};

void write_to_bible_dpbuf(DisplayBuf* buf, SWModule* bib);
void present_bible_dp(BibleDisplay* bibdp);
