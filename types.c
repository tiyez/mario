
#define Types_Inline
#include "types.h"

#include "log.h"

#include <stdlib.h>
#include <string.h>

static int	expand (void **pdata, int *max, int size) {
	int		new_max = *max <= 0 ? 1 : *max * 2;
	void	*data;

	data = realloc (*pdata, new_max * size);
	if (data) {
		*pdata = data;
		*max = new_max;
	}
	return (!!data);
}

int		new_tileset_resource (struct resources *res) {
	int		index;

	if (res->tilesets_count >= res->tilesets_max) {
		if (!expand ((void **) &res->tilesets, &res->tilesets_max, sizeof res->tilesets[0])) {
			Error ("cannot expand memory for tileset array");
			return (-1);
		}
	}
	index = res->tilesets_count;
	res->tilesets_count += 1;
	return (index);
}

int		new_map_resource (struct resources *res) {
	int		index;

	if (res->maps_count >= res->maps_max) {
		if (!expand ((void **) &res->maps, &res->maps_max, sizeof res->maps[0])) {
			Error ("cannot expand memory for map array");
			return (-1);
		}
	}
	index = res->maps_count;
	res->maps_count += 1;
	return (index);
}

void	free_resources (struct resources *res) {
	free (res->tilesets);
	free (res->maps);
	memset (res, 0, sizeof *res);
}



