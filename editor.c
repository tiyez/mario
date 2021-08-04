
#include "editor.h"

#include "defs.h"
#include "log.h"

#include <string.h>
#include <stdlib.h>

void	init_editor (struct editor *editor, struct resources *resources) {
	memset (editor, 0, sizeof *editor);
	editor->view = Editor_View_map;
	editor->resources = resources;
	editor->current_tileset = -1;
	editor->current_grid = -1;
	editor->current_tile = -1;
	editor->current_map = -1;
}

static void	init_editor_tileset_menu (struct editor *editor) {
	clear_menu (&editor->menu);
	for (int index = 0; index < editor->resources->tilesets_count; index += 1) {
		struct menu_entry	entry = {
			.type = Menu_Entry_button,
			.subtype = Editor_Menu_Subtype_tileset,
			.identity = index,
		};

		strncpy (entry.text, editor->resources->tilesets[index].filename, sizeof entry.text - 1);
		entry.text[sizeof entry.text - 1] = 0;
		add_menu_entry (&editor->menu, &entry);
	}
	invalidate_choice (&editor->menu_choice);
}

static void	init_editor_map_menu (struct editor *editor) {
	struct menu_entry	entry = {
		.type = Menu_Entry_button,
		.subtype = Editor_Menu_Subtype_map_new,
		.identity = 0,
	};

	strncpy (entry.text, "create new map", sizeof "create new map" - 1);
	clear_menu (&editor->menu);
	add_menu_entry (&editor->menu, &entry);
	for (int index = 0; index < editor->resources->maps_count; index += 1) {
		struct menu_entry	entry = {
			.type = Menu_Entry_button,
			.subtype = Editor_Menu_Subtype_map,
			.identity = index,
		};

		strncpy (entry.text, editor->resources->maps[index].filename, sizeof entry.text - 1);
		entry.text[sizeof entry.text - 1] = 0;
		add_menu_entry (&editor->menu, &entry);
	}
	invalidate_choice (&editor->menu_choice);
}

static void	run_editor_menu (struct editor *editor, struct editor_input *input) {
	const struct menu	*menu = &editor->menu;
	struct choice		*choice = &editor->menu_choice;

	if (menu->entries_count > 0) {
		if (input->up && choice->select > 0) {
			choice->select -= 1;
		}
		if (input->down && choice->select < menu->entries_count - 1) {
			choice->select += 1;
		}
		if (input->apply && choice->select >= 0) {
			const struct menu_entry	*entry = &menu->entries[choice->select];

			if (entry->subtype == Editor_Menu_Subtype_tileset) {
				editor->current_tileset = entry->identity;
				invalidate_choice (&editor->grid_choice);
				if (editor->resources->tilesets[editor->current_tileset].grids_count == 1) {
					editor->current_grid = 0;
					editor->grid_choice.select = 0;
					editor->current_tile = -1;
					invalidate_choice (&editor->tile_choice);
				} else {
					editor->current_grid = -1;
				}
			} else if (entry->subtype == Editor_Menu_Subtype_map) {
				editor->current_map = entry->identity;
				invalidate_choice (&editor->map_tile_choice);
			} else if (entry->subtype == Editor_Menu_Subtype_map_new) {
				char	*filename;

				printf ("Please, enter filename of the new map: assets/");
				if (scanf ("%ms", &filename) > 0) {
					int			index = new_map_resource (editor->resources);
					struct map	*map = &editor->resources->maps[index];

					memset (map, 0, sizeof *map);
					map->width = map->height = 1;
					map->tile_width = map->tile_height = 16;
					map->data = malloc (sizeof *map->data);
					*map->data = 0;
					map->filename = malloc (sizeof "assets/" + strlen (filename));
					map->filename[0] = 0;
					map->filename = strcat (map->filename, "assets/");
					map->filename = strcat (map->filename, filename);
					free (filename);
					editor->current_map = index;
				} else {
					Error ("invalid filename for new map");
				}
			} else {
				Error ("undefined menu entry subtype");
			}
			editor->is_menu = 0;
		}
	}
}

static int	get_grid_pixel_width (struct tilegrid *grid) {
	int		width = grid->tile_width * grid->width + grid->tile_padding_x * (grid->width - 1);

	return (width);
}

static int	get_grid_pixel_height (struct tilegrid *grid) {
	int		height = grid->tile_height * grid->height + grid->tile_padding_y * (grid->height - 1);

	return (height);
}

static void	run_editor_tileset_grid_choice (struct editor *editor, struct editor_input *input) {
	struct tileset	*tileset = &editor->resources->tilesets[editor->current_tileset];
	struct choice	*choice = &editor->grid_choice;

	if (tileset->grids_count > 0) {
		if (choice->select < 0) {
			choice->select = 0;
		}
		if (editor->is_alt && (choice->select >= 0)) {
			struct tilegrid	*grid = &tileset->grids[choice->select];

			if (input->left && grid->x > 0) {
				grid->x -= 1;
			}
			if (input->right && grid->x + get_grid_pixel_width (grid) < tileset->width - 1) {
				grid->x += 1;
			}
			if (input->up && grid->y > 0) {
				grid->y -= 1;
			}
			if (input->down && grid->y + get_grid_pixel_height (grid) < tileset->height - 1) {
				grid->y += 1;
			}
		} else {
			if (input->left) {
				choice->select -= 1;
				if (choice->select < 0) {
					choice->select = tileset->grids_count - 1;
				}
			}
			if (input->right) {
				choice->select += 1;
				if (choice->select >= tileset->grids_count) {
					choice->select = 0;
				}
			}
			if (input->apply && choice->select >= 0) {
				editor->current_grid = choice->select;
				editor->current_tile = -1;
				invalidate_choice (&editor->tile_choice);
			}
			if (input->cancel) {
				editor->current_grid = -1;
				editor->current_tileset = -1;
			}
		}
	}
}

static void	run_editor_2d_choicer (struct choice *choice, int width, int height, struct editor_input *input) {
	int		x = choice->select % width;
	int		y = choice->select / width;

	if (input->left && x > 0) {
		x -= 1;
	}
	if (input->right && x < width - 1) {
		x += 1;
	}
	if (input->up && y > 0) {
		y -= 1;
	}
	if (input->down && y < height - 1) {
		y += 1;
	}
	choice->select = y * width + x;
}

static void	run_editor_tileset_tile_choice (struct editor *editor, struct editor_input *input) {
	struct tileset	*tileset = &editor->resources->tilesets[editor->current_tileset];
	struct tilegrid	*grid = &tileset->grids[editor->current_grid];
	const int		tiles_count = grid->width * grid->height;

	if (tiles_count > 0) {
		if (editor->tile_choice.select < 0) {
			editor->tile_choice.select = 0;
		}
		if (!editor->is_alt && !editor->is_control && !editor->is_shift) {
			run_editor_2d_choicer (&editor->tile_choice, grid->width, grid->height, input);
			if (input->apply) {
				editor->current_tile = editor->tile_choice.select;
			}
			if (input->cancel) {
				editor->current_tile = -1;
				editor->current_grid = -1;
			}
		} else if (editor->is_alt) {
			if (input->left && grid->tile_width > 1) {
				grid->tile_width -= 1;
			}
			grid->tile_width += 1;
			if (input->right && grid->x + get_grid_pixel_width (grid) < tileset->width) {
				grid->tile_width += 1;
			}
			grid->tile_width -= 1;
			if (input->up && grid->tile_height > 1) {
				grid->tile_height -= 1;
			}
			grid->tile_height += 1;
			if (input->down && grid->y + get_grid_pixel_height (grid) < tileset->height) {
				grid->tile_height += 1;
			}
			grid->tile_height -= 1;
		} else if (editor->is_control) {
			int		sx = editor->tile_choice.select % grid->width;
			int		sy = editor->tile_choice.select / grid->width;
			int		cx = editor->current_tile % grid->width;
			int		cy = editor->current_tile / grid->width;

			if (input->left && grid->width > 1) {
				grid->width -= 1;
			}
			grid->width += 1;
			if (input->right && grid->x + get_grid_pixel_width (grid) < tileset->width) {
				grid->width += 1;
			}
			grid->width -= 1;
			if (input->up && grid->height > 1) {
				grid->height -= 1;
			}
			grid->height += 1;
			if (input->down && grid->y + get_grid_pixel_height (grid) < tileset->height) {
				grid->height += 1;
			}
			grid->height -= 1;
			sx = Min (sx, grid->width - 1);
			sy = Min (sy, grid->height - 1);
			cx = Min (cx, grid->width - 1);
			cy = Min (cy, grid->height - 1);
			editor->tile_choice.select = sy * grid->width + sx;
			if (editor->current_tile >= 0) {
				editor->current_tile = cy * grid->width + cx;
			}
		}
	}
}

static void	run_editor_tileset_view (struct editor *editor, struct editor_input *input) {
	if (input->menu || (editor->current_tileset < 0 && !editor->is_menu)) {
		editor->is_menu = !editor->is_menu;
		if (editor->is_menu) {
			init_editor_tileset_menu (editor);
		}
	}
	if (editor->is_menu) {
		run_editor_menu (editor, input);
	} else if (editor->current_tileset >= 0) {
		if (editor->current_grid < 0) {
			run_editor_tileset_grid_choice (editor, input);
		} else {
			run_editor_tileset_tile_choice (editor, input);
		}
	}
}

static unsigned	make_map_tile (int type, int tileset, int grid, int tile) {
	unsigned	result;

	Assert ((type & 0xFFFFFF00) == 0);
	Assert ((tileset & 0xFFFFFF00) == 0);
	Assert ((grid & 0xFFFFFFF0) == 0);
	Assert ((tile & 0xFFFFF000) == 0);
	result = (unsigned) type << 24;
	result += tileset << 16;
	result += grid << 12;
	result += tile;
	return (result);
}

static void	blit_map (unsigned *dest, int dwidth, int dheight, unsigned *src, int swidth, int sheight, int sx, int sy) {
	const int	start_y = Max (sy, 0);
	const int	start_x = Min (sx, 0);
	const int	start_ix = start_x - sx;
	const int	width = start_x + (swidth - start_ix) > dwidth ? dwidth - start_x : swidth - start_ix;

	if (width > 0 && start_ix < swidth) {
		for (int y = start_y, iy = start_y - sy; y < dheight && iy < sheight; y += 1, iy += 1) {
			unsigned	*dest_data = dest + y * dwidth + start_x;
			unsigned	*src_data = src + iy * swidth + start_ix;

			memcpy (dest_data, src_data, width * sizeof *src);
		}
	}
}

static void	expand_map (struct map *map, int x_vector, int y_vector) {
	const int	x_count = Absolute (x_vector);
	const int	x_dir = x_count == 0 ? 0 : x_vector / x_count;
	const int	x_blit = x_dir < 0 ? x_count : 0;
	const int	y_count = Absolute (y_vector);
	const int	y_dir = y_count == 0 ? 0 : y_vector / y_count;
	const int	y_blit = y_dir < 0 ? y_count : 0;
	const int	new_width = map->width + x_count;
	const int	new_height = map->height + y_count;
	const int	new_size = new_height * new_width;
	unsigned	*new_data;

	new_data = malloc (new_size * sizeof *new_data);
	if (new_data) {
		memset (new_data, 0, new_size * sizeof *new_data);
		blit_map (new_data, new_width, new_height, map->data, map->width, map->height, x_blit, y_blit);
		free (map->data);
		map->data = new_data;
		map->width = new_width;
		map->height = new_height;
	} else {
		Error ("cannot allocate memory for map when expanding it");
	}
}

static void	shrink_map (struct map *map, int x_vector, int y_vector) {
	const int	x_count = Absolute (x_vector);
	const int	x_dir = x_count == 0 ? 0 : x_vector / x_count;
	const int	x_blit = x_dir > 0 ? x_count : 0;
	const int	y_count = Absolute (y_vector);
	const int	y_dir = y_count == 0 ? 0 : y_vector / y_count;
	const int	y_blit = y_dir > 0 ? y_count : 0;
	const int	new_width = map->width - x_count;
	const int	new_height = map->height - y_count;
	const int	new_size = new_height * new_width;
	unsigned	*new_data;

	if (new_width > 0 && new_height > 0) {
		new_data = malloc (new_size * sizeof *new_data);
		if (new_data) {
			memset (new_data, 0, new_size * sizeof *new_data);
			blit_map (new_data, new_width, new_height, map->data, map->width, map->height, x_blit, y_blit);
			free (map->data);
			map->data = new_data;
			map->width = new_width;
			map->height = new_height;
		} else {
			Error ("cannot allocate memory for map when expanding it");
		}
	} else {
		Trace ("attempt to shrink map to zero width/height");
	}
}

static void	run_editor_map_expander (struct map *map, struct editor_input *input) {
	if (input->left) {
		expand_map (map, -1, 0);
	}
	if (input->right) {
		expand_map (map, 1, 0);
	}
	if (input->up) {
		expand_map (map, 0, -1);
	}
	if (input->down) {
		expand_map (map, 0, 1);
	}
}

static void	run_editor_map_shrinker (struct map *map, struct editor_input *input) {
	if (input->left) {
		shrink_map (map, -1, 0);
	}
	if (input->right) {
		shrink_map (map, 1, 0);
	}
	if (input->up) {
		shrink_map (map, 0, -1);
	}
	if (input->down) {
		shrink_map (map, 0, 1);
	}
}

static void	run_editor_map_edit (struct editor *editor, struct editor_input *input) {
	struct map	*map = &editor->resources->maps[editor->current_map];

	if (map->width > 0 && map->height > 0) {
		if (editor->is_control) {
			if (editor->is_shift) {
				run_editor_map_shrinker (map, input);
			} else {
				run_editor_map_expander (map, input);
			}
			if (input->erase) {
				memset (map->data, 0, map->width * map->height * sizeof *map->data);
			}
		} else {
			if (editor->map_tile_choice.select < 0) {
				editor->map_tile_choice.select = 0;
			}
			run_editor_2d_choicer (&editor->map_tile_choice, map->width, map->height, input);
			if (input->apply || input->erase) {
				unsigned	*tile = map->data + editor->map_tile_choice.select;

				if (input->apply && editor->current_tileset >= 0 && editor->current_grid >= 0 && editor->current_tile >= 0) {
					*tile = make_map_tile (1, editor->current_tileset, editor->current_grid, editor->current_tile);
				} else {
					*tile = 0;
				}
			}
			if (input->cancel) {
				editor->current_map = -1;
			}
		}
	}
}

static void	run_editor_map_view (struct editor *editor, struct editor_input *input) {
	if (input->menu || (editor->current_map < 0 && !editor->is_menu)) {
		editor->is_menu = !editor->is_menu;
		if (editor->is_menu) {
			init_editor_map_menu (editor);
		}
	}
	if (editor->is_menu) {
		run_editor_menu (editor, input);
	} else if (editor->current_map >= 0) {
		run_editor_map_edit (editor, input);
	}
}

void	run_editor (struct editor *editor, struct editor_input *input) {
	if (input->view) {
		editor->view += 1;
		if (editor->view == Editor_View__end) {
			editor->view = Editor_View__start;
		}
		editor->is_menu = 0;
	}
	if (input->control_on) {
		editor->is_control = 1;
	}
	if (input->control_off) {
		editor->is_control = 0;
	}
	if (input->shift_on) {
		editor->is_shift = 1;
	}
	if (input->shift_off) {
		editor->is_shift = 0;
	}
	if (input->alt_on) {
		editor->is_alt = 1;
	}
	if (input->alt_off) {
		editor->is_alt = 0;
	}
	if (input->save) {
		store_resources (editor->resources);
	}
	if (editor->view == Editor_View_map) {
		run_editor_map_view (editor, input);
	} else if (editor->view == Editor_View_tileset) {
		run_editor_tileset_view (editor, input);
	} else {
		Error ("invalid editor view");
	}
}

