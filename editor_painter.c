
#include "editor_painter.h"
#include "menu_painter.h"
#include "types.h"

#include "defs.h"
#include "log.h"
#include "settings.h"

#include <math.h>
#include <string.h>

void	init_editor_painter (struct editor_painter *painter, struct editor *editor, struct terminal *terminal) {
	memset (painter, 0, sizeof *painter);
	painter->editor = editor;
	painter->terminal = terminal;
}

void	adjust_map_coordinates (struct editor_painter *painter, int x, int y) {
	painter->focus_x = Lerp (painter->focus_x, x - Draw_Window_Width / 2 + 30, 0.3f);
	painter->focus_y = Lerp (painter->focus_y, y - Draw_Window_Height / 2 + 30, 0.3f);
	painter->focus_x = roundf (painter->focus_x * 100) / 100;
	painter->focus_y = roundf (painter->focus_y * 100) / 100;
}

void	extract_map_values (unsigned value, int *type, int *tileset, int *grid, int *tile) {
	*tile = value & 0xFFF;
	value >>= 12;
	*grid = value & 0xF;
	value >>= 4;
	*tileset = value & 0xFF;
	value >>= 8;
	*type = value & 0xFF;
}

void	draw_editor_map (struct framebuffer *buffer, struct editor_painter *painter) {
	struct editor	*editor = painter->editor;

	if (editor->current_map >= 0 && editor->current_map < editor->resources->maps_count) {
		struct map	*map = &editor->resources->maps[editor->current_map];
		const int	tile_px = (editor->map_tile_choice.select % map->width) * map->tile_width;
		const int	tile_py = (editor->map_tile_choice.select / map->width) * map->tile_height;
		int			focus_x, focus_y;

		adjust_map_coordinates (painter, tile_px, tile_py);
		focus_x = painter->focus_x;
		focus_y = painter->focus_y;
		for (int y = 0; y < map->height; y += 1) {
			for (int x = 0; x < map->width; x += 1) {
				unsigned	map_tile = *(map->data + y * map->width + x);
				int			type, tileset_index, grid_index, tile_index;

				extract_map_values (map_tile, &type, &tileset_index, &grid_index, &tile_index);
				if (tileset_index >= 0 && tileset_index < editor->resources->tilesets_count) {
					struct tileset	*tileset = &editor->resources->tilesets[tileset_index];

					if (grid_index >= 0 && grid_index < tileset->grids_count) {
						struct tilegrid	*grid = &tileset->grids[grid_index];
						const int	tile_px = (tile_index % grid->width) * grid->tile_width;
						const int	tile_py = (tile_index / grid->width) * grid->tile_height;

						draw_texture (buffer, &(struct frame) {
							.x = x - focus_x,
							.y = y - focus_y,
							.width = grid->tile_width,
							.height = grid->tile_height,
							.stride = tileset->stride,
						}, tileset->data + tile_py * tileset->stride + tile_px * tileset->channels);
					}
				}
			}
		}
		draw_linebox (buffer, tile_px - focus_x, tile_py - focus_y, map->tile_width, map->tile_height, 0xFFFFFFFF);
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

static int	get_grid_pixel_tile_x (struct tilegrid *grid, int x) {
	x = grid->tile_width * x + grid->tile_padding_x * x + grid->x;
	return (x);
}

static int	get_grid_pixel_tile_y (struct tilegrid *grid, int y) {
	y = grid->tile_height * y + grid->tile_padding_y * y + grid->y;
	return (y);
}

void	draw_editor_tileset (struct framebuffer *buffer, struct editor_painter *painter) {
	struct editor	*editor = painter->editor;

	if (editor->current_tileset >= 0 && editor->current_tileset < editor->resources->tilesets_count) {
		struct tileset	*tileset = &editor->resources->tilesets[editor->current_tileset];

		if (editor->current_grid >= 0 && editor->current_grid < tileset->grids_count) {
			struct tilegrid	*grid = &tileset->grids[editor->current_grid];
			const int	tile_px = get_grid_pixel_tile_x (grid, editor->tile_choice.select % grid->width);
			const int	tile_py = get_grid_pixel_tile_y (grid, editor->tile_choice.select / grid->width);

			if (editor->tile_choice.select >= 0) {
				adjust_map_coordinates (painter, tile_px, tile_py);
			} else {
				adjust_map_coordinates (painter, grid->x, grid->y);
			}
			const int	focus_x = roundf (painter->focus_x), focus_y = roundf (painter->focus_y);
			draw_texture (buffer, &(struct frame) {
				.x = -focus_x,
				.y = -focus_y,
				.width = tileset->width,
				.height = tileset->height,
				.stride = tileset->stride,
			}, tileset->data);
			if (editor->current_tile >= 0) {
				const int	tile_px = get_grid_pixel_tile_x (grid, editor->current_tile % grid->width);
				const int	tile_py = get_grid_pixel_tile_y (grid, editor->current_tile / grid->width);

				draw_linebox (buffer, tile_px - focus_x, tile_py - focus_y, grid->tile_width, grid->tile_height, 0xFF776666);
			}
			draw_linebox (buffer, tile_px - focus_x, tile_py - focus_y, grid->tile_width, grid->tile_height, 0xFFFFFFFF);
		} else if (editor->grid_choice.select >= 0) {
			struct tilegrid	*grid = &tileset->grids[editor->grid_choice.select];

			adjust_map_coordinates (painter, grid->x, grid->y);
			draw_texture (buffer, &(struct frame) {
				.x = -painter->focus_x,
				.y = -painter->focus_y,
				.width = tileset->width,
				.height = tileset->height,
				.stride = tileset->stride,
			}, tileset->data);
			draw_linebox (buffer, grid->x - painter->focus_x - 1, grid->y - painter->focus_y - 1, get_grid_pixel_width (grid) + 1, get_grid_pixel_height (grid) + 1, 0xFFFFAAFF);
		}
	}
	(void) editor, (void) buffer;
}

void	run_editor_painter (struct framebuffer *buffer, struct editor_painter *painter) {
	struct editor	*editor = painter->editor;

	if (editor->is_menu) {
		struct menu_painter	menu_painter = {
			.menu = &editor->menu,
			.terminal = painter->terminal,
			.selected = editor->menu_choice.select,
		};

		run_menu_painter (buffer, &menu_painter);
	} else {
		if (editor->view == Editor_View_map) {
			draw_editor_map (buffer, painter);
		} else if (editor->view == Editor_View_tileset) {
			draw_editor_tileset (buffer, painter);
		} else {
			Error ("unknown editor view");
		}
	}
}





