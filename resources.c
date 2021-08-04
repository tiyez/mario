
#include "resources.h"
#include "schema.h"
#include "file.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <stdio.h>
#include "log.h"

#include "defs.h"

int		create_tileset (struct resources *resources, struct schema_entry *entry) {
	int				result;
	struct tileset	tileset;

	tileset.data = stbi_load (entry->filename, &tileset.width, &tileset.height, &tileset.channels, 4);
	if (tileset.data) {
		tileset.channels = 4;
		tileset.stride = tileset.width * tileset.channels;
		tileset.size = tileset.height * tileset.stride;
		tileset.grids_count = entry->tilegrids_count;
		memcpy (tileset.grids, entry->tilegrids, sizeof entry->tilegrids[0] * entry->tilegrids_count);
		tileset.filename = strdup (entry->filename);
		if (tileset.filename) {
			int		tileset_index = new_tileset_resource (resources);

			if (tileset_index >= 0) {
				resources->tilesets[tileset_index] = tileset;
				result = 1;
			} else {
				Error ("cannot allocate new tileset resource");
				free (tileset.filename);
				tileset.filename = 0;
				free (tileset.data);
				tileset.data = 0;
				result = 0;
			}
			result = 1;
		} else {
			Error ("cannot duplicate filename of texture");
			free (tileset.data);
			tileset.data = 0;
			result = 0;
		}
	} else {
		Error ("cannot load image via stb_image");
		result = 0;
	}
	return (result);
}

int		create_map (struct resources *resources, struct schema_entry *entry) {
	struct map	map = {0};
	FILE		*file;
	int			result = 0;

	file = fopen (entry->filename, "rb");
	if (file) {
		struct map_data_header	header;

		if (fread (&header, sizeof header, 1, file) <= 0) {
			Error ("cannot read from map file '%s'", entry->filename);
			return 0;
		}
		map.width = header.width;
		map.height = header.height;
		map.tile_width = header.tile_width;
		map.tile_height = header.tile_height;
		map.data = malloc (map.width * map.height * sizeof *map.data);
		if (fread (map.data, sizeof *map.data, map.width * map.height, file) <= 0) {
			Error ("cannot read from map file '%s'", entry->filename);
		}
		map.filename = strdup (entry->filename);
		if (map.data && map.filename) {
			int		index = new_map_resource (resources);

			if (index >= 0) {
				resources->maps[index] = map;
				result = 1;
			} else {
				Error ("cannot allocate memory for map");
			}
		} else {
			Error ("cannot allocate memory for map");
			free (map.data);
			free (map.filename);
		}
		fclose (file);
	} else {
		Error ("cannot open map file '%s'", entry->filename);
	}
	return (result);
}

int		init_resources (struct resources *resources) {
	FILE	*schema_handle;
	int		schema_size;
	char	*schema_data;
	int		result;

	memset (resources, 0, sizeof *resources);
	(void) resources;
	schema_handle = fopen ("schema", "r");
	if (schema_handle) {
		schema_data = read_entire_file (schema_handle, &schema_size);
		fclose (schema_handle);
		schema_handle = 0;
		if (schema_data) {
			struct schema_entry	entry = {0};
			int					offset = 0, ret, count = 0;

			while (offset < schema_size && 0 < (ret = get_schema_entry (schema_data + offset, &entry))) {
				if (entry.type == Schema_Type_tileset) {
					if (create_tileset (resources, &entry)) {
						Info ("tileset %s loaded", entry.filename);
					} else {
						Error ("cannot load tileset %s", entry.filename);
					}
				} else if (entry.type == Schema_Type_map) {
					if (create_map (resources, &entry)) {
						Info ("map %s loaded", entry.filename);
					} else {
						Error ("cannot load map %s", entry.filename);
					}
				} else {
					Error ("unsupported schema type");
				}
				offset += ret;
				count += 1;
			}
			if (offset >= schema_size) {
				Info ("schema entries count: %d", count);
				result = 1;
			} else {
				if (schema_data[offset] == '\n') {
					Error ("empty line in schema file");
				} else {
					Error ("invalid schema file format");
				}
				result = 0;
			}
			free (schema_data);
		} else {
			Error ("cannot read schema file");
			result = 0;
		}
	} else {
		Error ("cannot open schema file");
		result = 0;
	}
	return (result);
}

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
		void	*ptr = res->tilesets;

		if (expand (&ptr, &res->tilesets_max, sizeof res->tilesets[0])) {
			res->tilesets = ptr;
		} else {
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
		void	*ptr = res->maps;

		if (expand (&ptr, &res->maps_max, sizeof res->maps[0])) {
			res->maps = ptr;
		} else {
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

void	store_resources (struct resources *res) {
	FILE	*schema_handle;

	schema_handle = fopen ("schema", "w");
	if (schema_handle) {
		for (int tileset_index = 0; tileset_index < res->tilesets_count; tileset_index += 1) {
			struct tileset	*tileset = &res->tilesets[tileset_index];

			fprintf (schema_handle, "tileset:%s\n", tileset->filename);
			for (int grid_index = 0; grid_index < tileset->grids_count; grid_index += 1) {
				struct tilegrid	*grid = &tileset->grids[grid_index];

				fprintf (schema_handle, "@ %d-%d %dx%d %d,%d %d,%d\n",
						grid->width, grid->height,
						grid->tile_width, grid->tile_height,
						grid->x, grid->y,
						grid->tile_padding_x, grid->tile_padding_y);
			}
		}
		for (int map_index = 0; map_index < res->maps_count; map_index += 1) {
			struct map	*map = &res->maps[map_index];
			FILE		*map_file;

			fprintf (schema_handle, "map:%s\n", map->filename);
			map_file = fopen (map->filename, "wb");
			if (map_file) {
				struct map_data_header	header = {
					.width = map->width,
					.height = map->height,
					.tile_width = map->tile_width,
					.tile_height = map->tile_height,
				};

				if (fwrite (&header, sizeof header, 1, map_file) <= 0) {
					Error ("cannot write to map file '%s'", map->filename);
				}
				if (fwrite (map->data, sizeof *map->data, map->width * map->height, map_file) <= 0) {
					Error ("cannot write to map file '%s'", map->filename);
				}
				fclose (map_file);
			} else {
				Error ("cannot open map file '%s'", map->filename);
			}
		}
	}
	fclose (schema_handle);
	schema_handle = 0;
}







