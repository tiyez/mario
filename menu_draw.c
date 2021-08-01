
#include "menu_draw.h"

#include "log.h"

void	draw_menu (struct framebuffer *buffer, struct menu *menu) {
	(void) buffer, (void) menu;

	// printf ("===== Make your choice =====\n");
	for (int index = 0; index < menu->entries_count; index += 1) {
		// printf ("%s%d: %s\n", "", index, menu->entries[index].text);
	}
}
