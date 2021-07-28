
#ifndef Editor_H
#define Editor_H

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



#endif /* Editor_H */
