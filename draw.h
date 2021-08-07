
#ifndef Draw_H
#define Draw_H

#include "types.h"

// draws RGBA texture to framebuffer
void	draw_colorbox (struct framebuffer *, int x, int y, int width, int height, unsigned color);
void	draw_texture (struct framebuffer *, struct frame *, unsigned char *data);
void	draw_texture_scaled (struct framebuffer *, struct frame *, float scale, unsigned char *data);
void	draw_line (struct framebuffer *buffer, int x, int y, int xend, int yend, unsigned color);
void	draw_linebox (struct framebuffer *, int x, int y, int width, int height, unsigned color);

void	draw_framebuffer (struct framebuffer *dest, struct framebuffer *src);

#endif /* Draw_H */
