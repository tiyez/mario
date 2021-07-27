
#ifndef Game_H
#define Game_H

#include <stddef.h>

struct input {
	int		is_left;
	int		is_right;
	int		is_jump;
	int		is_action;
};

enum block_type {
	Block_empty,

	Block_solid,
	Block_action,
};

struct block {
	enum block_type	type;
	int				texture_index;
};

struct world {
	int		width;
	int		height;

	struct block	*blocks;
	unsigned		(*textures)[16][16];
};

struct game {

	int		lives;

	int		world_index;

	struct world	*world;

};

void	init_game (struct game *);
int		run_game (struct game *, struct input *);

#endif /* Game_H */
