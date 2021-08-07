
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_gl.h"
#include "sokol_time.h"

#include "state.h"
#include "editor_painter.h"

#include "settings.h"

#include "log.h"

void	init_state (struct state *);
void	calculate_frame (struct state *);
void	cleanup (struct state *);
void	handle_event (const struct sapp_event *, struct state *);

sapp_desc	sokol_main(int argc, char* argv[]) {
	static struct state	state;

	stm_setup();
	state.arguments_count = argc;
	state.arguments = argv;
	if (!init_resources (&state.resources)) {
		Error ("cannot initialize resources");
		exit (1);
	}
	return (sapp_desc) {
		.width = Window_Width,
		.height = Window_Height,
		.high_dpi = true,
		.user_data = &state,
		.init_userdata_cb = (void (*)(void *)) init_state,
		.frame_userdata_cb = (void (*)(void *)) calculate_frame,
		.cleanup_userdata_cb = (void (*)(void *)) cleanup,
		.event_userdata_cb = (void (*)(const struct sapp_event *, void *)) handle_event,
		.window_title = Window_Title,
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

	sapp_set_window_title (Window_Title " (map editor)");
	init_terminal (&state->terminal);
	init_editor (&state->editor, &state->resources);
	init_editor_painter (&state->editor_painter, &state->editor, &state->terminal, &state->camera);
	init_entity_world (&state->world);
	state->entity_painter.world = &state->world;

	state->framebuffer.width = sapp_width() / Window_Scale;
	state->framebuffer.height = sapp_height() / Window_Scale;
	state->framebuffer.channels = 4;
	state->framebuffer.stride = state->framebuffer.width * state->framebuffer.channels;
	state->framebuffer.size = state->framebuffer.stride * state->framebuffer.height;
	state->framebuffer.data = malloc (state->framebuffer.size);

	state->screenbuffer.width = sapp_width();
	state->screenbuffer.height = sapp_height();
	state->screenbuffer.channels = 4;
	state->screenbuffer.stride = state->screenbuffer.width * state->screenbuffer.channels;
	state->screenbuffer.size = state->screenbuffer.stride * state->screenbuffer.height;
	state->screenbuffer.data = malloc (state->screenbuffer.size);

	memset (state->screenbuffer.data, Clear_Brightness, state->screenbuffer.size);
	// memset (&state->input, 0, sizeof state->input);

	state->img = sg_make_image(&(sg_image_desc){
								.width = state->screenbuffer.width,
								.height = state->screenbuffer.height,
								.usage = SG_USAGE_DYNAMIC,
									// .data.subimage[0][0] = (sg_range) { .ptr = state->buffer, .size = state->buffer_size },
								});

}

void	calculate_frame (struct state *state) {

	memset (state->framebuffer.data, Clear_Brightness, state->framebuffer.size);

	run_editor (&state->editor, &state->editor_input);
	run_editor_painter (&state->framebuffer, &state->editor_painter);
	run_entity_world (&state->world, &state->entity_input);
	state->entity_painter.width = 1024;
	state->entity_painter.height = 64;
	state->entity_painter.focus_x = 0;
	state->entity_painter.focus_y = 0;
	run_entity_painter (&state->framebuffer, &state->entity_painter);

	if (state->editor_input.view) {
		if (state->editor.view == Editor_View_map) {
			sapp_set_window_title (Window_Title " (map editor)");
		} else if (state->editor.view == Editor_View_tileset) {
			sapp_set_window_title (Window_Title " (tileset editor)");
		}
	}

	memset (&state->editor_input, 0, sizeof state->editor_input);
	memset (&state->entity_input, 0, sizeof state->entity_input);

	if (1) {
		draw_framebuffer (&state->screenbuffer, &state->framebuffer);
		sg_update_image(state->img, &(sg_image_data) {
							.subimage[0][0] = (sg_range) { .ptr = state->screenbuffer.data, .size = state->screenbuffer.size },
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
		sgl_v3f_t2f(state->screenbuffer.width, 0, 0,  1.0f,  1.0f);
		sgl_v3f_t2f(state->screenbuffer.width, state->screenbuffer.height, 0,  1.0f, 0);
		sgl_v3f_t2f(0, state->screenbuffer.height, 0, 0, 0);
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
	static int	is_control;

	(void) state;
	switch (event->type) {
		case SAPP_EVENTTYPE_KEY_DOWN: {
			switch (event->key_code) {
				case SAPP_KEYCODE_LEFT_CONTROL:
				case SAPP_KEYCODE_RIGHT_CONTROL:
					state->editor_input.control_on = 1;
					is_control = 1;
				break ;
				case SAPP_KEYCODE_LEFT_SHIFT:
				case SAPP_KEYCODE_RIGHT_SHIFT:
					state->editor_input.shift_on = 1;
				break ;
				case SAPP_KEYCODE_LEFT_ALT:
				case SAPP_KEYCODE_RIGHT_ALT:
					state->editor_input.alt_on = 1;
				break ;
				case SAPP_KEYCODE_BACKSPACE:
					state->editor_input.erase = 1;
				break ;
				case SAPP_KEYCODE_TAB:
					state->editor_input.view = 1;
				break ;
				case SAPP_KEYCODE_Z:
				case SAPP_KEYCODE_ENTER:
					state->editor_input.apply = 1;
					state->editor_input.apply_on = 1;
				break ;
				case SAPP_KEYCODE_ESCAPE:
					state->editor_input.cancel = 1;
				break ;
				case SAPP_KEYCODE_SPACE:
					state->editor_input.menu = 1;
				break ;
				case SAPP_KEYCODE_LEFT:
					state->editor_input.left = 1;
					state->entity_input.left = 1;
				break ;
				case SAPP_KEYCODE_RIGHT:
					state->editor_input.right = 1;
					state->entity_input.right = 1;
				break ;
				case SAPP_KEYCODE_UP:
					state->editor_input.up = 1;
					state->entity_input.up = 1;
				break ;
				case SAPP_KEYCODE_DOWN:
					state->editor_input.down = 1;
				break ;
				case SAPP_KEYCODE_S:
					if (is_control) {
						state->editor_input.save = 1;
					}
				break ;
				default: break ;
			}
		} break ;
		case SAPP_EVENTTYPE_KEY_UP: {
			switch (event->key_code) {
				case SAPP_KEYCODE_LEFT_CONTROL:
				case SAPP_KEYCODE_RIGHT_CONTROL:
					state->editor_input.control_off = 1;
					is_control = 0;
				break ;
				case SAPP_KEYCODE_LEFT_SHIFT:
				case SAPP_KEYCODE_RIGHT_SHIFT:
					state->editor_input.shift_off = 1;
				break ;
				case SAPP_KEYCODE_LEFT_ALT:
				case SAPP_KEYCODE_RIGHT_ALT:
					state->editor_input.alt_off = 1;
				break ;
				case SAPP_KEYCODE_Z:
				case SAPP_KEYCODE_ENTER:
					state->editor_input.apply_off = 1;
				break ;
				case SAPP_KEYCODE_LEFT:
					state->entity_input.left_off = 1;
				break ;
				case SAPP_KEYCODE_RIGHT:
					state->entity_input.right_off = 1;
				break ;
				default: break ;
			}
		}
		case SAPP_EVENTTYPE_CHAR: {
		} break ;
		default: {
		} break ;
	}
}
