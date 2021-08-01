
#include "menu.h"

#include "defs.h"
#include "log.h"

#include <stdlib.h>

int		add_menu_entry (struct menu *menu, const struct menu_entry *entry) {
	if (menu->entries_count >= menu->entries_max) {
		const int	expand_to = Max (1, menu->entries_max * 2);
		void		*ret;

		ret = realloc (menu->entries, expand_to * sizeof *entry);
		if (ret != 0) {
			menu->entries = ret;
			menu->entries_max = expand_to;
		} else {
			Error ("cannot expand memory for new menu entry");
			return (0);
		}
	}
	menu->entries[menu->entries_count] = *entry;
	menu->entries_count += 1;
	return (1);
}

void	clear_menu (struct menu *menu) {
	menu->entries_count = 0;
}

void	free_menu_resources (struct menu *menu) {
	free (menu->entries);
	menu->entries_max = 0;
	menu->entries_count = 0;
}

