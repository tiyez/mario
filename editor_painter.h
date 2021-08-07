
#ifndef Editor_Painter_H
#define Editor_Painter_H

#include "editor.h"
#include "draw.h"

struct editor_painter {
	struct editor	*editor;
	struct terminal	*terminal;
	struct camera	*camera;
};

void	init_editor_painter (struct editor_painter *, struct editor *, struct terminal *, struct camera *);
void	run_editor_painter (struct framebuffer *, struct editor_painter *);

#endif /* Editor_Painter_H */
