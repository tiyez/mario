

#include <stdlib.h>
#include <stdio.h>

#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_gl.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include "c/c11ast.h"
#include "types.h"

float		clampf(float value);
unsigned	clampu8(unsigned value);
size_t		string_length (const char *str);

#define Start_Code 0x01
#define Tab_Size 4

struct state {
	int				arguments_count;
	char               **arguments;
	int				need_redraw;
	int				buffer_width;
	int				buffer_height;
	int				buffer_stride;
	int				buffer_size;
	unsigned char	*buffer;
	sg_image		img;
    
	void					*fontdata;
	struct stbtt_fontinfo	fontinfo;
	float					fontsize;
	float					fontscale;
    
	int				bitmap_width;
	int				bitmap_height;
	int				bitmap_stride;
	unsigned char	*bitmap;
    
	char			text[256];
	size_t			text_size;
    
	int      glyph_indices[(0x7e - Start_Code) + 1];
	struct stbtt_kerningentry	glyph_kerning[1024];
};

void	init_state (struct state *);
void	calculate_frame (struct state *);
void	cleanup (struct state *);
void	handle_event (const struct sapp_event *, struct state *);

sapp_desc	sokol_main(int argc, char* argv[]) {
	static struct state	state;
    
	state.arguments_count = argc;
	state.arguments = argv;
	state.text_size = 0;
	return (sapp_desc) {
		.width = 800,
		.height = 600,
		.high_dpi = true,
		.user_data = &state,
		.init_userdata_cb = (void (*)(void *)) init_state,
		.frame_userdata_cb = (void (*)(void *)) calculate_frame,
		.cleanup_userdata_cb = (void (*)(void *)) cleanup,
		.event_userdata_cb = (void (*)(const struct sapp_event *, void *)) handle_event,
		.window_title = "c_code_editor",
	};
}

void	memset_uint (void *ptr, unsigned int value, int size) {
	for (int i = 0; i < size; i += 1) {
		((unsigned int *)ptr)[i] = value;
	}
}

void	get_text_location(const char *p, const char *where, stb_lex_location *loc)
{
    int line_number = 1;
    int char_offset = 0;
    while (*p && p < where) {
        if (*p == '\n' || *p == '\r') {
            p += (p[0]+p[1] == '\r'+'\n' ? 2 : 1); // skip newline
            line_number += 1;
            char_offset = 0;
        } else {
            char_offset += (*p == '\t') * 3 + 1;
            ++p;
        }
    }
    loc->line_number = line_number;
    loc->line_offset = char_offset + 1;
}

void	init_state (struct state *state) {
	Unused (state);
    
	sg_setup (&(sg_desc){
                  .context = sapp_sgcontext()
              });
	sgl_setup (&(sgl_desc_t){
                   .sample_count = sapp_sample_count()
               });
	state->buffer_width = sapp_width();
	state->buffer_height = sapp_height();
	state->buffer_stride = state->buffer_width * 4;
	state->buffer_size = state->buffer_stride * state->buffer_height;
	state->buffer = malloc (state->buffer_size);
    
	for (int i = 0; i < state->buffer_height / 2; i += 1) {
		memset_uint (state->buffer + i * state->buffer_stride, 0xFF000000, state->buffer_width);
	}
	for (int i = state->buffer_height / 2; i < state->buffer_height; i += 1) {
		memset_uint (state->buffer + i * state->buffer_stride, 0xFF00FF00, state->buffer_width);
	}
    
	state->img = sg_make_image(&(sg_image_desc){
                                   .width = state->buffer_width,
                                   .height = state->buffer_height,
                                   .usage = SG_USAGE_DYNAMIC,
                                   //	.data.subimage[0][0] = (sg_range) { .ptr = state->buffer, .size = state->buffer_size },
                               });
    
    
	FILE	*fontfile = fopen ("JetBrainsMono-Regular.ttf", "r");
	if (!fontfile) {
		printf ("fopen error\n");
		exit (1);
	}
	fseek (fontfile, 0, SEEK_END);
	size_t filesize = ftell (fontfile);
	fseek (fontfile, 0, SEEK_SET);
    
	state->fontdata = malloc (filesize);
	if (fread (state->fontdata, filesize, 1, fontfile) != 1) {
		printf ("fread error\n");
		exit (1);
	}
    
	if (stbtt_InitFont (&state->fontinfo, state->fontdata, 0) == 0) {
		printf ("FontInit error\n");
		exit (1);
	}
    
	state->fontsize = 32 * sapp_dpi_scale ();
	state->fontscale = stbtt_ScaleForPixelHeight (&state->fontinfo, state->fontsize);
    
	int		x0, x1, y0, y1;
	int		width = 0, height = 0;
	for (size_t i = 0; i < Array_Count (state->glyph_indices); i += 1) {
		int		unicode_codepoint = i + Start_Code;
        
		state->glyph_indices[i] = stbtt_FindGlyphIndex (&state->fontinfo, unicode_codepoint);
		if (state->glyph_indices[i] != 0) {
			stbtt_GetGlyphBitmapBox (&state->fontinfo, state->glyph_indices[i], state->fontscale, state->fontscale, &x0, &y0, &x1, &y1);
			width = Max(width, x1 - x0);
			height = Max(height, y1 - y0);
		}
        
#if 0
		if (state->glyph_indices[i] == 0) {
			printf ("glyph for '%c'(%x) does not exists\n", (char) unicode_codepoint, unicode_codepoint);
		} else if (unicode_codepoint < 0x20) {
			printf ("!!!!! %d\n", unicode_codepoint);
		}
#endif
	}
    
	printf ("width: %d; height: %d;\n", width, height);
	state->bitmap_width = width;
	state->bitmap_height = height;
	state->bitmap_stride = (width + 3) & -4;
	state->bitmap = malloc (state->bitmap_stride * state->bitmap_height);
    
	state->need_redraw = 1;
    
#if 0
	FILE *file = fopen ("test.c", "r");
	fseek (file, 0, SEEK_END);
	size_t  file_size = ftell (file);
	fseek (file, 0, SEEK_SET);
	char *memory = malloc (file_size + 1);
	struct c11ast c11ast = create_c11ast();
	if (1 <= fread (memory, file_size, 1, file)) {
		memory[file_size] = 0;
		struct c11ast_result result = c11ast_build (&c11ast.memory, memory, file_size);
		if (!result.ok) {
			stb_lex_location	loc;
            
			get_text_location (memory, result.token_first_char, &loc);
			printf ("C11AST Error[%d:%d]: %s\n", loc.line_number, loc.line_offset, result.error_message);
		}
	}
	free (memory);
#else
	// unsigned	build_arithmetic_op_ast (void);
	// build_arithmetic_op_ast ();
	// void	c_op_test (const char *filename);
	// c_op_test ("test/testfile_c_op.c");
	// void	c_flow_test (const char *filename);
	// c_flow_test ("test/testfile_c_flow.c");
	// void c_declaration_test (const char *filename);
	// c_declaration_test ("test/testfile_c_declaration.c");
#endif
    
	// exit (0);
}

unsigned	blend_rgba(const unsigned uback, const unsigned ufront) {
	unsigned char	result[4];
	const unsigned char	*back = (const unsigned char *)&uback;
	const unsigned char	*front = (const unsigned char *)&ufront;
	float		alpha = front[3] / 255.f;
	float		rev_alpha = 1 - alpha;
    
	result[0] = clampu8(clampf(alpha * (front[0] / 255.f) + rev_alpha * (back[0] / 255.f)) * 255.f);
	result[1] = clampu8(clampf(alpha * (front[1] / 255.f) + rev_alpha * (back[1] / 255.f)) * 255.f);
	result[2] = clampu8(clampf(alpha * (front[2] / 255.f) + rev_alpha * (back[2] / 255.f)) * 255.f);
	result[3] = 0xFF;
	return *(unsigned *)result;
}

void	calculate_frame (struct state *state) {
	Unused (state);
    
	memset (state->buffer, 0, state->buffer_stride * state->buffer_height);
    
	int		pen_x_start = (20 * sapp_dpi_scale ()) / state->fontscale;
	int		pen_x = pen_x_start, pen_y = 200 * sapp_dpi_scale ();
	for (size_t i = 0; i < state->text_size; i += 1) {
		int		code = state->text[i];
        
		if (code > 0x20 && code <= 0x7e) {
			int		glyph_index = state->glyph_indices[code - Start_Code];
            
			int		x0, y0, x1, y1;
			stbtt_GetGlyphBitmapBox (&state->fontinfo, glyph_index, state->fontscale, state->fontscale, &x0, &y0, &x1, &y1);
			int		width = x1 - x0, height = y1 - y0;
			int		advance, left_side_bearing;
            
			stbtt_GetGlyphHMetrics (&state->fontinfo, glyph_index, &advance, &left_side_bearing);
            
			int		additional_advance = 0;
			if (i > 0 && state->text[i] > 0x20 && state->text[i] <= 0x7e) {
				additional_advance = stbtt_GetGlyphKernAdvance (&state->fontinfo, state->glyph_indices[state->text[i - 1] - Start_Code], glyph_index);
			}
            
			float	pen_xf = (pen_x + additional_advance + left_side_bearing) * state->fontscale;
			int	x_off = pen_xf;
			int		y_off = pen_y + y0;
			float	subpixel_x = pen_xf - x_off;
            
            
			stbtt_MakeGlyphBitmapSubpixel (&state->fontinfo, state->bitmap, width, height, state->bitmap_stride, state->fontscale, state->fontscale, subpixel_x, 0, glyph_index);
            
			for (int y = 0; y < height; y += 1) {
				unsigned char	*bitmap_line = state->bitmap + state->bitmap_stride * y;
				unsigned int	*buffer_line = (unsigned int *) (state->buffer + state->buffer_stride * (y + y_off)) + x_off;
                
				for (int x = 0; x < width; x += 1) {
					unsigned	bitmap_value = bitmap_line[x];
					unsigned	bitmap_color = 0xFF + (0xFF << 8) + (0xFF << 16) + (bitmap_value << 24);
					// unsigned	bitmap_color = bitmap_value + (bitmap_value << 8) + (bitmap_value << 16) + (bitmap_value << 24);
                    
					buffer_line[x] = blend_rgba (buffer_line[x], bitmap_color);
					// buffer_line[x] = bitmap_color;
				}
			}
            
			pen_x += advance;
		} else if (code == ' ' || code == '\t') {
			int		glyph_index = state->glyph_indices[code - Start_Code];
            
			if (glyph_index != 0) {
				int		advance, left_side_bearing;
				stbtt_GetGlyphHMetrics (&state->fontinfo, glyph_index, &advance, &left_side_bearing);
				pen_x += advance;
			} else if (code == '\t' && (glyph_index = state->glyph_indices[' ' - Start_Code]) != 0) {
				int		advance, left_side_bearing;
				stbtt_GetGlyphHMetrics (&state->fontinfo, glyph_index, &advance, &left_side_bearing);
				pen_x += advance * Tab_Size;
			}
		} else if (code == '\n') {
			int		ascent, descent, line_gap;
            
			stbtt_GetFontVMetrics (&state->fontinfo, &ascent, &descent, &line_gap);
			pen_y += (ascent - descent + line_gap) * state->fontscale;
			pen_x = pen_x_start;
		}
	}
    
    
	if (state->need_redraw) {
		sg_update_image(state->img, &(sg_image_data) {
                            .subimage[0][0] = (sg_range) { .ptr = state->buffer, .size = state->buffer_size },
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
		sgl_v3f_t2f(state->buffer_width, 0, 0,  1.0f,  1.0f);
		sgl_v3f_t2f(state->buffer_width, state->buffer_height, 0,  1.0f, 0);
		sgl_v3f_t2f(0, state->buffer_height, 0, 0, 0);
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
	Unused (state);
}

int		is_valid_text_char (unsigned code) {
	return ((code >= 0x20 && code <= 0x7e) || code == '\t');
}

void	handle_event (const struct sapp_event *event, struct state *state) {
	Unused (state);
    
	switch (event->type) {
		case SAPP_EVENTTYPE_KEY_DOWN: {
			if (event->key_code == SAPP_KEYCODE_BACKSPACE) {
				if (state->text_size > 0) {
					state->text_size -= 1;
				}
			} else if (event->key_code == SAPP_KEYCODE_ENTER) {
				if (state->text_size < Array_Count (state->text)) {
					state->text[state->text_size] = '\n';
					state->text_size += 1;
				}
			}
		} break ;
		case SAPP_EVENTTYPE_CHAR: {
			if (is_valid_text_char (event->char_code) && state->text_size < Array_Count (state->text)) {
				state->text[state->text_size] = event->char_code;
				state->text_size += 1;
			}
		} break ;
		default: {
		} break ;
	}
}


