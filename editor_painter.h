
#ifndef Editor_Painter_H
#define Editor_Painter_H

#include "editor.h"
#include "draw.h"

struct editor_painter {
	struct editor	*editor;
	struct terminal	*terminal;

	float	focus_x;
	float	focus_y;
};

void	init_editor_painter (struct editor_painter *, struct editor *, struct terminal *);
void	run_editor_painter (struct framebuffer *, struct editor_painter *);

#endif /* Editor_Painter_H */
