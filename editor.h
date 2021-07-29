
#ifndef Editor_H
#define Editor_H

struct editor_input {
	int		left;
	int		right;
	int		up;
	int		down;
	int		apply;
	int		cancel;
	int		erase;
	int		menu;
};

enum editor_view {
	Editor_View_map,
	Editor_View_tileset,
};

struct editor {
	enum editor_view	view;

	int					current_tileset;
	int					current_grid;
	int					current_tile;

	int					current_map;

	int					tilesets_count;
	struct tileset		*tilesets;
	int					maps_count;
	struct map			*maps;
};

void	init_editor (struct editor *);
void	run_editor (struct editor *, struct editor_input *);

#endif /* Editor_H */
