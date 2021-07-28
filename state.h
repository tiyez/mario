
#ifndef State_H
#define State_H

#include "draw.h"
#include "core.h"

struct state {
	int		arguments_count;
	char	**arguments;

	int					need_redraw;
	struct framebuffer	framebuffer;
	sg_image			img;

	struct core			core;
};

#endif /* State_H */
