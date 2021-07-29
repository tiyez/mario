
#ifndef Core_H
#define Core_H

#include "types.h"

struct world {

	int				tilesets_count;
	struct tileset	tilesets[16];

};

int		init_world (struct world *);

#endif /* Core_H */
