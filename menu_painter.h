
#ifndef Menu_Draw_H
#define Menu_Draw_H

#include "menu.h"
#include "draw.h"
#include "terminal.h"

struct menu_painter {
	struct menu		*menu;
	struct terminal	*terminal;

	int				selected;
};

void	run_menu_painter (struct framebuffer *, struct menu_painter *);

#endif /* Menu_Draw_H */
