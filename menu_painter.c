
#include "menu_painter.h"

#include "log.h"

void	run_menu_painter (struct framebuffer *buffer, struct menu_painter *painter) {
	struct menu	*menu = painter->menu;
	(void) buffer, (void) menu;

	clear_terminal (painter->terminal);
	printf ("===== Make your choice (%d) =====\n", menu->entries_count);
	for (int index = 0; index < menu->entries_count; index += 1) {
		printf ("%s%d: %s\n", index == painter->selected ? "> " : "", index, menu->entries[index].text);
	}
}
