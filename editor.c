
#include "editor.h"

#include "defs.h"
#include "log.h"

#include <string.h>

void	init_editor (struct editor *editor) {
	memset (editor, 0, sizeof *editor);
	editor->view = Editor_View_map;
}

enum menu_entry_subtype {
	Menu_SubType_tileset,
};

void	init_editor_tileset_menu (struct editor *editor) {
	clear_menu (&editor->menu);
	for (int index = 0; index < editor->tilesets_count; index += 1) {
		struct menu_entry	entry = {
			.type = Menu_Entry_button,
			.subtype = Menu_SubType_tileset,
			.identity = index,
		};

		strncpy (entry.button.text, editor->tilesets[index].filename, sizeof entry.button.text - 1);
		entry.button.text[sizeof entry.button.text - 1] = 0;
		add_menu_entry (&editor->menu, &entry);
	}
}

void	run_editor_tileset_menu (struct editor *editor, struct editor_input *input) {
	const struct menu	*menu = &editor->menu;
	struct choice		*choice = &editor->menu_choice;

	if (menu->entries_count > 0) {
		if (input->up) {
			choice->select = Max (0, choice->select - 1);
		}
		if (input->down) {
			choice->select = Min (menu->entries_count - 1, choice->select + 1);
		}
		if (input->apply && choice->select >= 0) {
			const struct menu_entry	*entry = &menu->entries[choice->select];

			if (entry->subtype == Menu_SubType_tileset) {
				editor->current_tileset = entry->identity;
				invalidate_choice (&editor->grid_choice);
			} else {
				Error ("undefined menu entry subtype");
			}
			editor->is_menu = 0;
		}
	}
}

void	run_editor_tileset_view (struct editor *editor, struct editor_input *input) {
	if (input->menu) {
		editor->is_menu = !editor->is_menu;
		if (editor->is_menu) {
			init_editor_tileset_menu (editor);
		}
	}
	if (editor->is_menu) {
		run_editor_tileset_menu (editor, input);
		return ;
	}

}

void	run_editor_map_view (struct editor *editor, struct editor_input *input) {
	(void) editor, (void) input;
}

void	run_editor (struct editor *editor, struct editor_input *input) {
	if (editor->view == Editor_View_map) {
		run_editor_map_view (editor, input);
	} else if (editor->view == Editor_View_tileset) {
		run_editor_tileset_view (editor, input);
	} else {
		Error ("invalid editor view");
	}
}

