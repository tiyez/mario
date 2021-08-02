
#ifndef Editor_H
#define Editor_H

#include "types.h"
#include "resources.h"
#include "menu.h"

struct editor_input {
	int		left;
	int		right;
	int		up;
	int		down;
	int		apply;
	int		cancel;
	int		erase;
	int		menu;
	int		view;
	int		control_on;
	int		control_off;
	int		shift_on;
	int		shift_off;
};

enum editor_view {
	Editor_View_map,
	Editor_View_tileset,

	Editor_View__end,
	Editor_View__start = Editor_View_map,
};

enum editor_menu_subtype {
	Editor_Menu_Subtype_,
	Editor_Menu_Subtype_tileset,
	Editor_Menu_Subtype_map,
	Editor_Menu_Subtype_map_new,
};

struct editor {
	enum editor_view	view;

	int					is_menu;
	int					is_control;
	int					is_shift;

	struct menu			menu;

	struct choice		menu_choice;
	struct choice		grid_choice;
	struct choice		tile_choice;
	struct choice		map_tile_choice;

	int					current_tileset;
	int					current_grid;
	int					current_tile;

	int					current_map;
	int					is_map_resize;

	struct resources	*resources;
};

void	init_editor (struct editor *, struct resources *);
void	run_editor (struct editor *, struct editor_input *);

#endif /* Editor_H */
