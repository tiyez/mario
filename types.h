
#ifndef Types_H
#define Types_H

struct input {
	int		is_left;
	int		is_right;
	int		is_jump;
	int		is_action;
};

enum block_type {
	Block_empty,

	Block_solid,
	Block_action,
};

struct block {
	enum block_type	type;
	int				texture_index;
};

struct map {
	int		width;
	int		height;

	struct block	*blocks;
	unsigned		(*textures)[16][16];
};

struct tilegrid {
	int		width;
	int		height;
	int		tile_width;
	int		tile_height;
	int		tile_offset_x;
	int		tile_offset_y;
	int		tile_padding_x;
	int		tile_padding_y;
};

struct tileset {
	int		width;
	int		height;
	int		stride;
	int		channels;
	int		size;

	int				grids_count;
	struct tilegrid	grids[8];

	unsigned char	*data;
};

struct framebuffer {
	int		width;
	int		height;
	int		size;
	int		stride;
	int		channels;

	unsigned char	*data;
};

// RGBA texture frame
struct frame {
	int		x;
	int		y;
	int		width;
	int		height;
	int		stride;
};

#endif /* Types_H */
