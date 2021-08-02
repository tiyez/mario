
#include "menu_painter.h"

#include "log.h"

#include <string.h>

void	run_menu_painter (struct framebuffer *buffer, struct menu_painter *painter) {
	static struct menu	s_remember;
	static int s_select;
	struct menu	*menu = painter->menu;
	(void) buffer, (void) menu;

	if (memcmp (&s_remember, menu, sizeof *menu) != 0 || s_select != painter->selected) {
		s_remember = *menu;
		s_select = painter->selected;
		clear_terminal (painter->terminal);
		printf ("===== Make your choice (%d) =====\n", menu->entries_count);
		for (int index = 0; index < menu->entries_count; index += 1) {
			printf ("%s%d: %s\n", index == painter->selected ? "> " : "", index, menu->entries[index].text);
		}
	}
}
