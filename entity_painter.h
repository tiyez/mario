

#ifndef Entity_Painter
#define Entity_Painter

#include "draw.h"
#include "entity.h"

struct entity_painter {
	struct entity_world	*world;

	int		focus_x;
	int		focus_y;
	int		width;
	int		height;
};

void	run_entity_painter (struct framebuffer *, struct entity_painter *);

#endif /* Entity_Painter */
