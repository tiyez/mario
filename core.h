
#ifndef Core_H
#define Core_H

#include "editor.h"


struct core {

	int				tilesets_count;
	struct tileset	tilesets[16];

};

int		init_core (struct core *);

#endif /* Core_H */
