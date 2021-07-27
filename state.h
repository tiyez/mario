
#ifndef State_H
#define State_H

#include "draw.h"
#include "game.h"

struct state {
	int		arguments_count;
	char	**arguments;

	int					need_redraw;
	struct framebuffer	framebuffer;
	sg_image			img;

	struct input		input;
	struct game			game;
};

#endif /* State_H */
