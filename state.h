
#ifndef State_H
#define State_H

#include "draw.h"
#include "resources.h"
#include "editor.h"
#include "editor_painter.h"
#include "terminal.h"
#include "entity.h"
#include "entity_painter.h"

struct state {
	int		arguments_count;
	char	**arguments;

	int					need_redraw;
	struct framebuffer	framebuffer;
	struct framebuffer	screenbuffer;
	sg_image			img;

	struct resources		resources;
	struct camera			camera;
	struct editor			editor;
	struct editor_painter	editor_painter;
	struct editor_input		editor_input;
	struct entity_world		world;
	struct entity_painter	entity_painter;
	struct entity_input		entity_input;

	struct terminal			terminal;
};

#endif /* State_H */
