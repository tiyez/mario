
#ifndef Types_H
#define Types_H

#ifndef Types_Inline
#define Types_Inline inline
#endif

struct input {
	int		is_left;
	int		is_right;
	int		is_jump;
	int		is_action;
};

enum block_type {
	Block_empty,

	Block_solid,
	Block_background,
};

struct map {
	int		width;
	int		height;
	int		tile_width;
	int		tile_height;

	/* 8 bit - type, 8 bit - tileset, 4 bit - grid, 12 bit - tile */
	unsigned	*data;

	char	*filename;
};

struct map_data_header {
	unsigned	width;
	unsigned	height;
	unsigned	tile_width;
	unsigned	tile_height;
	unsigned	reserved[16];
};

struct tilegrid {
	int		x;
	int		y;
	int		width;
	int		height;
	int		tile_width;
	int		tile_height;
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

	char			*filename;
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

struct choice {
	int		select;
};

Types_Inline void	invalidate_choice (struct choice *choice) {
	choice->select = -1;
}

#endif /* Types_H */
