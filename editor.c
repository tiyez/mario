
#include "editor.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <stdio.h>

void	*read_entire_file (FILE *handle, int *size) {
	void	*result;

	fseek (handle, 0, SEEK_END);
	*size = ftell (handle);
	if (*size > 0) {
		result = malloc (*size + 1);
		fseek (handle, 0, SEEK_SET);
		if (result && fread (result, *size, 1, handle) != *size) {
			free (result);
			result = 0;
		}
	} else {
		result = 0;
	}
	return (result);
}

struct schema_entry {
	char	*filename;
	int		tile_width;
	int		tile_height;
	int		tile_offset_x;
	int		tile_offset_y;
	int		tile_padding_x;
	int		tile_padding_y;
};

int		get_schema_entry (char *data, struct schema_entry *out) {
	int		result;
	int		filename_size;
	char	*ext;

	if ((ext = strstr (data, ".png"))) {
		filename_size = (ext - data) + 4;
	} else if ((ext = strstr (data, ".jpg"))) {
		filename_size = (ext - data) + 4;
	} else if ((ext = strstr (data, ".bmp"))) {
		filename_size = (ext - data) + 4;
	} else {
		return (-1);
	}

	char	*ptr = data + filename_size;

	out->tile_width = strtol (ptr, &ptr, 10);
	ptr += 1;
	out->tile_height = strtol (ptr, &ptr, 10);

	out->tile_offset_x = strtol (ptr, &ptr, 10);
	ptr += 1;
	out->tile_offset_y = strtol (ptr, &ptr, 10);

	out->tile_padding_x = strtol (ptr, &ptr, 10);
	ptr += 1;
	out->tile_padding_y = strtol (ptr, &ptr, 10);

	int line_width = 0;
	while (data[line_width] && data[line_width] != '\n') {
		line_width += 1;
	}

	if (data + line_width >= ptr) {
		char	*filename = malloc (filename_size + 1);

		if (filename) {
			memcpy (filename, data, filename_size);
			filename[filename_size] = 0;
			out->filename = filename;
			result = data[line_width] ? line_width + 1 : line_width;
		} else {
			result = -1;
		}
	} else {
		result = -1;
	}
	return (result);
}

int		init_editor (struct editor *editor) {
	FILE	*schema_handle;
	int		schema_size;
	char	*schema_data;
	int		result;

	schema_handle = fopen ("assets/.schema", "r");
	schema_data = read_entire_file (schema_handle, &schema_size);
	if (schema_data) {

	} else {
		result = 0;
	}
	return (result);
}









