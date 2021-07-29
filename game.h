
#ifndef Game_H
#define Game_H

#include <stddef.h>
#include "types.h"

struct game {

	int		lives;

	int		world_index;

	struct map	*map;

};

void	init_game (struct game *);
int		run_game (struct game *, struct input *);

#endif /* Game_H */
