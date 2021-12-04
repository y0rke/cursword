#pragma once
#include <cwchar>
#include <swmodule.h>
#include <display.h>

using namespace sword;

void scroll_down(display_win* buf, SWModule* module, int lines);
void scroll_up(display_win* buf, SWModule* module, int lines);
