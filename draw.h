
#ifndef Draw_H
#define Draw_H

#include "game.h"

struct framebuffer {
	int		width;
	int		height;
	int		size;
	int		stride;
	int		channels;

	unsigned char	*data;
};

void	draw_game (struct framebuffer *, struct game *);

#endif /* Draw_H */
