
#ifndef Entity_H
#define Entity_H

#include <stddef.h>

struct entity_input {
	int		left;
	int		left_off;
	int		right;
	int		right_off;
	int		up;
};

struct player {
	float	px;
	float	py;
	float	vx;
	float	vy;
	int		width;
	int		height;

	int		is_grounded;
};

struct ground {
	int		x;
	int		y;
	int		width;
	int		height;
};

struct entity_world {
	size_t			last_time;

	int				is_left;
	int				is_right;

	struct player	player;
	int				grounds_count;
	struct ground	grounds[64];
};

void	init_entity_world (struct entity_world *);
void	run_entity_world (struct entity_world *, struct entity_input *);

#endif /* Entity_H */
