
#include "editor.h"

void	init_editor (struct editor *editor) {
	editor->view = Editor_View_map;
	editor->current_tileset = 0;
	editor->current_grid = 0;
	editor->current_tile = 0;
	editor->current_map = 0;
}

void	run_editor (struct editor *editor, struct editor_input *input) {
	
}

