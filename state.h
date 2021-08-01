
#ifndef State_H
#define State_H

#include "draw.h"
#include "resources.h"
#include "editor.h"
#include "editor_painter.h"

struct state {
	int		arguments_count;
	char	**arguments;

	int					need_redraw;
	struct framebuffer	framebuffer;
	sg_image			img;

	struct resources		resources;
	struct editor			editor;
	struct editor_painter	editor_painter;
	struct editor_input		editor_input;
};

#endif /* State_H */
