
#include "entity_painter.h"

void	run_entity_painter (struct framebuffer *buffer, struct entity_painter *painter) {
	struct entity_world	*world = painter->world;
	int		player_x = (int) world->player.px - painter->focus_x;
	int		player_y = (painter->height - (int) world->player.py) - painter->focus_y;

	draw_colorbox (buffer, player_x, player_y, world->player.width, world->player.height, 0xFFFF0000);
}


