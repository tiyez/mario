
#ifndef Schema_H
#define Schema_H

#include "editor.h"

struct schema_entry {
	enum { Schema_Type_tileset } type;
	char	filename[128];
	int				tilegrids_count;
	struct tilegrid	tilegrids[8];
};

int		get_schema_entry (char *data, struct schema_entry *out);

#endif /* Schema_H */

