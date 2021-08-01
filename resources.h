
#ifndef Resources_H
#define Resources_H

#include "types.h"

struct resources {
	int					tilesets_count;
	int					tilesets_max;
	struct tileset		*tilesets;

	int					maps_count;
	int					maps_max;
	struct map			*maps;
};

int		init_resources (struct resources *);
int		new_tileset_resource (struct resources *);
int		new_map_resource (struct resources *);
void	free_resources (struct resources *);

#endif /* Resources_H */
