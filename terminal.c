
#include "terminal.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <termcap.h>

void	init_terminal (struct terminal *terminal) {
	memset (terminal, 0, sizeof *terminal);
	tgetent (terminal->term_desc, getenv ("TERM"));
	terminal->clear = tgetstr ("cl", 0);
}

void	clear_terminal (struct terminal *terminal) {
	fputs (terminal->clear, stdout);
}



