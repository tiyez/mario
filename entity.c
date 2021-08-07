
#include "entity.h"
#include "sokol_time.h"


void	init_entity_world (struct entity_world *world) {
	world->last_time = 0;
	world->player.px = 32;
	world->player.py = 128;
	world->player.vx = 0;
	world->player.vy = 0;
	world->player.width = 16;
	world->player.height = 16;
	world->grounds_count = 1;
	world->grounds[0].x = 0;
	world->grounds[0].y = 0;
	world->grounds[0].width = 64;
	world->grounds[0].height = 32;
}

#define Gravity 9.8f

static void	fall_player (struct player *player, double dt) {
	player->vy = player->vy - Gravity * dt;
}

static void	apply_player_movement (struct player *player) {
	player->px += player->vx;
	player->py += player->vy;
}

static int	is_player_collide_ground_y (struct player *player, struct ground *ground) {
	float	py = player->py + player->vy;
	int		result;

	if (py < ground->y + ground->height && py >= ground->y) {
		result = 1;
	} else if (py + player->height > ground->y && py + player->height <= ground->y + ground->height) {
		result = 1;
	} else {
		result = 0;
	}
	return (result);
}

static void	move_player (struct player *player, double dt, struct entity_input *input) {
	const float	speed = 2;

	(void) dt;
	player->vx = (input->right - input->left) * speed;
	if (input->up && player->is_grounded) {
		player->vy = 4;
	}
}

void	run_entity_world (struct entity_world *world, struct entity_input *input) {
	double	dt = stm_sec (stm_laptime (&world->last_time));

	if (input->left) {
		world->is_left = 1;
	}
	if (input->left_off) {
		world->is_left = 0;
	}
	if (input->right) {
		world->is_right = 1;
	}
	if (input->right_off) {
		world->is_right = 0;
	}
	input->left = world->is_left;
	input->right = world->is_right;

	move_player (&world->player, dt, input);
	fall_player (&world->player, dt);
	world->player.is_grounded = 0;
	for (int ground_index = 0; ground_index < world->grounds_count; ground_index += 1) {
		struct ground	*ground = &world->grounds[ground_index];
		float	px = world->player.px + world->player.vx;

		if (px >= ground->x && px < ground->x + ground->width) {
			if (world->player.px >= ground->x && world->player.px < ground->x + ground->width) {
				if (is_player_collide_ground_y (&world->player, ground)) {
					if (world->player.vy < 0) {
						world->player.py = ground->y + ground->height;
					} else {
						world->player.py = ground->y - world->player.height;
					}
					world->player.vy = 0;
					world->player.is_grounded = 1;
				}
			} else {
				if (world->player.vx < 0) {
					world->player.px = ground->x + ground->width;
				} else {
					world->player.px = ground->x - world->player.width;
				}
				world->player.vx = 0;
			}
		}
	}
	apply_player_movement (&world->player);

}





