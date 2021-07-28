
#include "file.h"

#include "log.h"

#include <stdlib.h>

void	*read_entire_file (FILE *handle, int *size) {
	void	*result;

	fseek (handle, 0, SEEK_END);
	*size = ftell (handle);
	if (*size > 0) {
		result = malloc (*size + 1);
		fseek (handle, 0, SEEK_SET);
		if (result && fread (result, 1, *size, handle) != (unsigned) *size) {
			free (result);
			result = 0;
			Error ("cannot read file");
		}
	} else {
		Error ("file has zero size");
		result = 0;
	}
	return (result);
}

