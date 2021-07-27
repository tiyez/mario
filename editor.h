
#ifndef Editor_H
#define Editor_H

struct tileset {
	int		width;
	int		height;
	int		stride;
	int		channels;
	int		size;

	int		tile_width;
	int		tile_height;
	int		tile_offset_x;
	int		tile_offset_y;
	int		tile_padding_x;
	int		tile_padding_y;

	unsigned char	*data;
};

struct editor {

	int				tileset_count;
	struct tileset	tileset[16];

};










void	init_editor (struct editor *);

#endif /* Editor_H */
