
#include "schema.h"
#include <string.h>
#include <stdlib.h>

#include "log.h"
#include "defs.h"

int		get_schema_entry (char *data, struct schema_entry *out) {
	char	*filename;
	int		filename_size;
	char	*ext;
	char	*ptr = data;

	memset (out, 0, sizeof *out);
	if (strncmp (ptr, "tileset:", 8) == 0) {
		out->type = Schema_Type_tileset;
		ptr += 8;
	} else {
		Error ("invalid schema entry type");
		return (-1);
	}

	filename = ptr;
	if ((ext = strstr (ptr, ".png"))) {
		filename_size = (ext - ptr) + 4;
	} else if ((ext = strstr (ptr, ".jpg"))) {
		filename_size = (ext - ptr) + 4;
	} else if ((ext = strstr (ptr, ".bmp"))) {
		filename_size = (ext - ptr) + 4;
	} else {
		Error ("unsupported filename extension");
		return (-1);
	}

	if (filename_size < (int) sizeof (out->filename) - 1) {
		memcpy (out->filename, ptr, filename_size);
		out->filename[filename_size] = 0;
	} else {
		Error ("filename too long");
		return (-1);
	}

	ptr += filename_size;
	while (*ptr && *ptr != '\n') {
		ptr += 1;
	}
	ptr += !!*ptr;

	if (out->type == Schema_Type_tileset) {
		while (*ptr == '@') {
			int		index;
			struct tilegrid	*tilegrid;

			if (out->tilegrids_count >= Array_Count (out->tilegrids)) {
				Error ("tileset grids list overflow");
				return (-1);
			}
			index = out->tilegrids_count;
			out->tilegrids_count += 1;
			tilegrid = &out->tilegrids[index];

			ptr += 1;
			tilegrid->width = strtol (ptr, &ptr, 10);
			if (*ptr != '-') {
				Error ("invalid tileset grid format");
				return (-1);
			}
			ptr += 1;
			tilegrid->height = strtol (ptr, &ptr, 10);

			tilegrid->tile_width = strtol (ptr, &ptr, 10);
			if (*ptr != 'x') {
				Error ("invalid tileset grid format");
				return (-1);
			}
			ptr += 1;
			tilegrid->tile_height = strtol (ptr, &ptr, 10);

			tilegrid->x = strtol (ptr, &ptr, 10);
			if (*ptr != ',') {
				Error ("invalid tileset grid format");
				return (-1);
			}
			ptr += 1;
			tilegrid->y = strtol (ptr, &ptr, 10);

			tilegrid->tile_padding_x = strtol (ptr, &ptr, 10);
			if (*ptr != ',') {
				Error ("invalid tileset grid format");
				return (-1);
			}
			ptr += 1;
			tilegrid->tile_padding_y = strtol (ptr, &ptr, 10);

			while (*ptr && *ptr != '\n') {
				ptr += 1;
			}
			ptr += !!*ptr;
		}
	}

	printf ("grids_count: %d\n", out->tilegrids_count);
	return (ptr - data);
}




