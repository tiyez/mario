
#include "core.h"
#include "schema.h"
#include "file.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <stdio.h>
#include "log.h"

#include "defs.h"

int		create_tileset (struct core *core, struct schema_entry *entry) {
	int		result;

	if (core->tilesets_count < Array_Count (core->tilesets)) {
		int				index;
		struct tileset	*tileset;

		index = core->tilesets_count;
		core->tilesets_count += 1;
		tileset = &core->tilesets[index];
		tileset->data = stbi_load (entry->filename, &tileset->width, &tileset->height, &tileset->channels, 4);
		if (tileset->data) {
			tileset->stride = tileset->width * tileset->channels;
			tileset->size = tileset->height * tileset->stride;
			tileset->grids_count = entry->tilegrids_count;
			memcpy (tileset->grids, entry->tilegrids, sizeof entry->tilegrids[0] * entry->tilegrids_count);
			result = 1;
		} else {
			Error ("cannot load image via stb_image");
			result = 0;
		}
		if (!result) {
			core->tilesets_count -= 1;
		}
	} else {
		Error ("tileset array overflow");
		result = 0;
	}
	return (result);
}

int		init_core (struct core *core) {
	FILE	*schema_handle;
	int		schema_size;
	char	*schema_data;
	int		result;

	(void) core;
	schema_handle = fopen ("schema", "r");
	if (schema_handle) {
		schema_data = read_entire_file (schema_handle, &schema_size);
		if (schema_data) {
			struct schema_entry	entry;
			int					offset = 0, ret, count = 0;

			while (schema_data[offset] && 0 < (ret = get_schema_entry (schema_data + offset, &entry))) {
				if (entry.type == Schema_Type_tileset) {
					if (create_tileset (core, &entry)) {
						Info ("tileset %s loaded", entry.filename);
					} else {
						Error ("cannot create tileset %s", entry.filename);
					}
				}
				offset += ret;
				count += 1;
			}
			if (!schema_data[offset]) {
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









