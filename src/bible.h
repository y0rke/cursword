#pragma once
#include "display.h"
#include <swmodule.h>

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

void write_to_bible_buf(DisplayBuf* buf, SWModule* bib);
