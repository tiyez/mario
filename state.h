
#ifndef State_H
#define State_H

#include "draw.h"
#include "world.h"

struct state {
	int		arguments_count;
	char	**arguments;

	int					need_redraw;
	struct framebuffer	framebuffer;
	sg_image			img;

	struct world		world;
};

#endif /* State_H */
