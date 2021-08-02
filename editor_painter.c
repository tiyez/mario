
#include "editor_painter.h"
#include "menu_painter.h"
#include "types.h"

#include "defs.h"
#include "log.h"

#include <math.h>
#include <string.h>

void	init_editor_painter (struct editor_painter *painter, struct editor *editor, struct terminal *terminal) {
	memset (painter, 0, sizeof *painter);
	painter->editor = editor;
	painter->terminal = terminal;
}

void	adjust_map_coordinates (struct editor_painter *painter, int x, int y) {
	const int		focus_box_radius = 0;
	float			dir_x = 0;
	float			dir_y = 0;
	float			len;

	dir_x = (painter->focus_x + focus_box_radius < x) - (painter->focus_x - focus_box_radius > x);
	dir_y = (painter->focus_y + focus_box_radius < y) - (painter->focus_y - focus_box_radius > y);
	len = dir_x * dir_x + dir_y * dir_y;
	if (len > 0) {
		len = sqrtf (len);
		dir_x /= len;
		dir_y /= len;

		painter->focus_x = Lerp (painter->focus_x, x, dir_x);
		painter->focus_y = Lerp (painter->focus_y, y, dir_y);
	}
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

		adjust_map_coordinates (painter, tile_px, tile_py);
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
							.x = x - painter->focus_x,
							.y = y - painter->focus_y,
							.width = grid->tile_width,
							.height = grid->tile_height,
							.stride = tileset->stride,
						}, tileset->data + tile_py * tileset->stride + tile_px * tileset->channels);
					}
				}
			}
		}
	}
}

void	draw_editor_tileset (struct framebuffer *buffer, struct editor_painter *painter) {
	struct editor	*editor = painter->editor;

	if (editor->current_tileset >= 0 && editor->current_tileset < editor->resources->tilesets_count) {
		struct tileset	*tileset = &editor->resources->tilesets[editor->current_tileset];

		if (editor->current_grid >= 0 && editor->current_grid < tileset->grids_count) {
			struct tilegrid	*grid = &tileset->grids[editor->current_grid];
			const int	tile_px = (editor->tile_choice.select % grid->width) * grid->tile_width;
			const int	tile_py = (editor->tile_choice.select / grid->width) * grid->tile_height;

			adjust_map_coordinates (painter, tile_px, tile_py);
			draw_texture (buffer, &(struct frame) {
				.x = -painter->focus_x,
				.y = -painter->focus_y,
				.width = tileset->width,
				.height = tileset->height,
				.stride = tileset->stride,
			}, tileset->data);
		} else {
			draw_texture (buffer, &(struct frame) {
				.x = -painter->focus_x,
				.y = -painter->focus_y,
				.width = tileset->width,
				.height = tileset->height,
				.stride = tileset->stride,
			}, tileset->data);
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





