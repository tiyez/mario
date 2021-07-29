
#ifndef Draw_H
#define Draw_H

#include "types.h"

// draws RGBA texture to framebuffer
void	draw_texture (struct framebuffer *, struct frame *, unsigned char *data);
void	draw_texture_scaled (struct framebuffer *, struct frame *, float scale, unsigned char *data);

#endif /* Draw_H */
