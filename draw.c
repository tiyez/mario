
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
			void	*src = data + (y - start_y) * frame->stride + (frame->x - start_x) * 4;

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













