
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_gl.h"
#include "sokol_time.h"

#include "state.h"
#include "draw.h"

void	init_state (struct state *);
void	calculate_frame (struct state *);
void	cleanup (struct state *);
void	handle_event (const struct sapp_event *, struct state *);

sapp_desc	sokol_main(int argc, char* argv[]) {
	static struct state	state;

	stm_setup();
	state.arguments_count = argc;
	state.arguments = argv;
	init_game (&state.game);
	return (sapp_desc) {
		.width = 800,
		.height = 600,
		.high_dpi = true,
		.user_data = &state,
		.init_userdata_cb = (void (*)(void *)) init_state,
		.frame_userdata_cb = (void (*)(void *)) calculate_frame,
		.cleanup_userdata_cb = (void (*)(void *)) cleanup,
		.event_userdata_cb = (void (*)(const struct sapp_event *, void *)) handle_event,
		.window_title = "TETRIS",
	};
}

void	init_state (struct state *state) {
	(void) state;

	sg_setup (&(sg_desc){
					.context = sapp_sgcontext()
				});
	sgl_setup (&(sgl_desc_t){
					.sample_count = sapp_sample_count()
				});
	state->framebuffer.width = sapp_width();
	state->framebuffer.height = sapp_height();
	state->framebuffer.channels = 4;
	state->framebuffer.stride = state->framebuffer.width * state->framebuffer.channels;
	state->framebuffer.size = state->framebuffer.stride * state->framebuffer.height;
	state->framebuffer.data = malloc (state->framebuffer.size);

	memset (state->framebuffer.data, 255, state->framebuffer.size);
	memset (&state->input, 0, sizeof state->input);

	state->img = sg_make_image(&(sg_image_desc){
								.width = state->framebuffer.width,
								.height = state->framebuffer.height,
								.usage = SG_USAGE_DYNAMIC,
									// .data.subimage[0][0] = (sg_range) { .ptr = state->buffer, .size = state->buffer_size },
								});

}

void	calculate_frame (struct state *state) {
	int	need_redraw;

	need_redraw = run_game (&state->game, &state->input);
	memset (&state->input, 0, sizeof state->input);

	if (!state->game.is_play) {
		init_game (&state->game);
	}

	if (need_redraw) {
		draw_game (&state->framebuffer, &state->game);

		sg_update_image(state->img, &(sg_image_data) {
							.subimage[0][0] = (sg_range) { .ptr = state->framebuffer.data, .size = state->framebuffer.size },
						});


		sgl_viewport(0, 0, sapp_width(), sapp_height(), true);
		sgl_defaults();
		sgl_default_pipeline();
		sgl_matrix_mode_projection();
		sgl_ortho(0, sapp_widthf(), 0, sapp_heightf(), -10, 10);

		sgl_enable_texture();
		sgl_texture(state->img);

		sgl_begin_quads();
		sgl_v3f_t2f(0, 0, 0, 0,  1.0f);
		sgl_v3f_t2f(state->framebuffer.width, 0, 0,  1.0f,  1.0f);
		sgl_v3f_t2f(state->framebuffer.width, state->framebuffer.height, 0,  1.0f, 0);
		sgl_v3f_t2f(0, state->framebuffer.height, 0, 0, 0);
		sgl_end();

		sg_begin_default_pass(&(sg_pass_action) {
				.colors[0] = {
					.action = SG_ACTION_CLEAR,
					.value = { 0.0f, 0.0f, 0.0f, 1.0f }
				}
			}, sapp_width(), sapp_height());

		sgl_draw();
		sg_end_pass();
		sg_commit();
	}
}

void	cleanup (struct state *state) {
	(void) state;
}

void	handle_event (const struct sapp_event *event, struct state *state) {
	(void) state;
	switch (event->type) {
		case SAPP_EVENTTYPE_KEY_DOWN: {
			if (event->key_code == SAPP_KEYCODE_LEFT) {
				state->input.is_left = 1;
			} else if (event->key_code == SAPP_KEYCODE_RIGHT) {
				state->input.is_right = 1;
			} else if (event->key_code == SAPP_KEYCODE_DOWN) {
				state->input.is_sprint_down = 1;
			} else if (event->key_code == SAPP_KEYCODE_SPACE) {
				state->input.is_rotate_left = 1;
			} else if (event->key_code == SAPP_KEYCODE_ENTER) {
				state->input.is_rotate_right = 1;
			}
		} break ;
		case SAPP_EVENTTYPE_KEY_UP: {
			if (event->key_code == SAPP_KEYCODE_DOWN) {
				state->input.is_sprint_up = 1;
			}
		}
		case SAPP_EVENTTYPE_CHAR: {
		} break ;
		default: {
		} break ;
	}
}
