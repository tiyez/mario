
#include "draw.h"
#include "game.h"

#include <string.h>
#include <stdio.h>

#include "defs.h"

void	draw_texture (struct framebuffer *buffer, struct frame *frame, unsigned char *data) {
	const int	start_x = Max (frame->x, 0);
	const int	end_x = Min (frame->width + frame->x, buffer->width);
	const int	start_y = Max (frame->y, 0);
	const int	end_y = Min (frame->height + frame->y, buffer->height);
	const int	frame_width = end_x - start_x;

	Assert (buffer->channels == 4);
	if (frame_width > 0) {
		for (int y = start_y; y < end_y; y += 1) {
			void	*dest = buffer->data + y * buffer->stride + start_x * buffer->channels;
			void	*src = data + (y - frame->y) * frame->stride + (start_x - frame->x) * 4;

			memcpy (dest, src, 4 * frame_width);
		}
	}
}

void	draw_texture_scaled (struct framebuffer *buffer, struct frame *frame, float scale, unsigned char *data) {
	const int	start_x_normal = Max (frame->x, 0);
	const int	start_x = start_x_normal * scale;
	const int	end_x = Min ((frame->width + frame->x) * scale, buffer->width);
	const int	start_y_normal = Max (frame->y, 0);
	const int	start_y = start_y_normal * scale;
	const int	end_y = Min ((frame->height + frame->y) * scale, buffer->height);
	const int	offset_x = -start_x + (start_x_normal - frame->x) * scale;
	const int	offset_y = -start_y + (start_y_normal - frame->y) * scale;
	const float	inv_scale = 1 / scale;

	Assert (buffer->channels == 4);
	for (int y = start_y; y < end_y; y += 1) {
		for (int x = start_x; x < end_x; x += 1) {
			void	*dest = buffer->data + y * buffer->stride + x * buffer->channels;
			void	*src = data + (int)((y + offset_y) * inv_scale) * frame->stride + (int)((x + offset_x) * inv_scale) * buffer->channels;

			memcpy (dest, src, 4);
		}
	}
}

enum {
	Cohen_Left = 1,
	Cohen_Right = 2,
	Cohen_Bottom = 4,
	Cohen_Top = 8,
};

static unsigned	compute_cohen_code (float x, float y, float xmax, float ymax) {
	unsigned	code = 0;

	if (x < 0) {
		code |= Cohen_Left;
	} else if (x > xmax) {
		code |= Cohen_Right;
	}
	if (y < 0) {
		code |= Cohen_Bottom;
	} else if (y > ymax) {
		code |= Cohen_Top;
	}
	return (code);
}

struct clipped {
	int		accept;
	float	x, y, xend, yend;
};

struct clipped	clip_line (float x, float y, float xend, float yend, float xmax, float ymax) {
	unsigned	cohen = compute_cohen_code (x, y, xmax, ymax);
	unsigned	cohen_end = compute_cohen_code (xend, yend, xmax, ymax);
	int			accept = 0;

	while (1) {
		if ((cohen | cohen_end) == 0) {
			accept = 1;
			break ;
		} else if ((cohen & cohen_end) != 0) {
			break ;
		} else {
			unsigned	outside = Max (cohen, cohen_end);
			float		nx, ny;

			if (outside & Cohen_Top) {
				nx = x + (xend - x) * (ymax - y) / (yend - y);
				ny = ymax;
			} else if (outside & Cohen_Bottom) {
				nx = x + (xend - x) * (0 - y) / (yend - y);
				ny = 0;
			} else if (outside & Cohen_Right) {
				ny = y + (yend - y) * (xmax - x) / (xend - x);
				nx = xmax;
			} else if (outside & Cohen_Left) {
				ny = y + (yend - y) * (0 - x) / (xend - x);
				nx = 0;
			}
			if (outside == cohen) {
				x = nx;
				y = ny;
				cohen = compute_cohen_code (x, y, xmax, ymax);
			} else {
				xend = nx;
				yend = ny;
				cohen_end = compute_cohen_code (xend, yend, xmax, ymax);
			}
		}
	}
	return (struct clipped) { accept, x, y, xend, yend };
}

void	draw_line (struct framebuffer *buffer, int x, int y, int xend, int yend, unsigned color) {
	struct clipped	c = clip_line (x, y, xend, yend, buffer->width - 1, buffer->height - 1);
	float			px, py, dx, dy, step;
	int				i;

	if (!c.accept) {
		return ;
	}
	dx = c.xend - c.x;
	dy = c.yend - c.y;
	if (Absolute (dx) >= Absolute (dy)) {
		step = Absolute (dx);
	} else {
		step = Absolute (dy);
	}
	dx = dx / step;
	dy = dy / step;
	px = c.x;
	py = c.y;
	i = 1;
	while (i <= step) {
		void	*ptr = buffer->data + (int) py * buffer->stride + (int) px * buffer->channels;

		memcpy (ptr, &color, 4);
		px += dx;
		py += dy;
		i += 1;
	}
}

// void	draw_linebox (struct framebuffer *buffer, int x, int y, int width, int height, unsigned color) {

// }

void	draw_framebuffer (struct framebuffer *dest, struct framebuffer *src) {
	const float	x_scale = (float) src->width / dest->width;
	const float	y_scale = (float) src->height / dest->height;

	Assert (dest->channels == src->channels);
	for (int y = 0; y < dest->height; y += 1) {
		for (int x = 0; x < dest->width; x += 1) {
			void	*dptr = dest->data + y * dest->stride + x * dest->channels;
			void	*sptr = src->data + (int) (y * y_scale) * src->stride + (int) (x * x_scale) * src->channels;

			memcpy (dptr, sptr, dest->channels);
		}
	}
}












