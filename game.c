
#include "game.h"

#include "sokol_time.h"

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define Array_Count(arr) (int)(sizeof (arr) / sizeof ((arr)[0]))

unsigned char	s_holdings[][4][4][4] = {
	{
		{
			{ 0, Color_Cyan, 0, 0 }, { 0, Color_Cyan, 0, 0 }, { 0, Color_Cyan, 0, 0 }, { 0, Color_Cyan, 0, 0 },
		},
		{
			{ 0, 0, 0, 0 }, { Color_Cyan, Color_Cyan, Color_Cyan, Color_Cyan }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
		},
		{
			{ 0, Color_Cyan, 0, 0 }, { 0, Color_Cyan, 0, 0 }, { 0, Color_Cyan, 0, 0 }, { 0, Color_Cyan, 0, 0 },
		},
		{
			{ 0, 0, 0, 0 }, { Color_Cyan, Color_Cyan, Color_Cyan, Color_Cyan }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
		},
	},
	{
		{
			{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, Color_Blue, 0, 0 }, { 0, Color_Blue, Color_Blue, Color_Blue },
		},
		{
			{ 0, 0, 0, 0 }, { 0, 0, Color_Blue, 0 }, { 0, 0, Color_Blue, 0 }, { 0, Color_Blue, Color_Blue, 0 },
		},
		{
			{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { Color_Blue, Color_Blue, Color_Blue, 0 }, { 0, 0, Color_Blue, 0 },
		},
		{
			{ 0, 0, 0, 0 }, { 0, Color_Blue, Color_Blue, 0 }, { 0, Color_Blue, 0, 0 }, { 0, Color_Blue, 0, 0 },
		},
	},
	{
		{
			{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, Color_Orange, 0 }, { Color_Orange, Color_Orange, Color_Orange, 0 },
		},
		{
			{ 0, 0, 0, 0 }, { 0, Color_Orange, Color_Orange, 0 }, { 0, 0, Color_Orange, 0 }, { 0, 0, Color_Orange, 0 },
		},
		{
			{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, Color_Orange, Color_Orange, Color_Orange }, { 0, Color_Orange, 0, 0 },
		},
		{
			{ 0, 0, 0, 0 }, { 0, Color_Orange, 0, 0 }, { 0, Color_Orange, 0, 0 }, { 0, Color_Orange, Color_Orange, 0 },
		},
	},
	{
		{
			{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, Color_Yellow, Color_Yellow, 0 }, { 0, Color_Yellow, Color_Yellow, 0 },
		},
		{
			{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, Color_Yellow, Color_Yellow, 0 }, { 0, Color_Yellow, Color_Yellow, 0 },
		},
		{
			{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, Color_Yellow, Color_Yellow, 0 }, { 0, Color_Yellow, Color_Yellow, 0 },
		},
		{
			{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, Color_Yellow, Color_Yellow, 0 }, { 0, Color_Yellow, Color_Yellow, 0 },
		},
	},
	{
		{
			{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, Color_Green, Color_Green }, { 0, Color_Green, Color_Green, 0 },
		},
		{
			{ 0, 0, 0, 0 }, { 0, Color_Green, 0, 0 }, { 0, Color_Green, Color_Green, 0 }, { 0, 0, Color_Green, 0 },
		},
		{
			{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, Color_Green, Color_Green }, { 0, Color_Green, Color_Green, 0 },
		},
		{
			{ 0, 0, 0, 0 }, { 0, Color_Green, 0, 0 }, { 0, Color_Green, Color_Green, 0 }, { 0, 0, Color_Green, 0 },
		},
	},
	{
		{
			{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, Color_Purple, 0 }, { 0, Color_Purple, Color_Purple, Color_Purple },
		},
		{
			{ 0, 0, 0, 0 }, { 0, 0, Color_Purple, 0 }, { 0, Color_Purple, Color_Purple, 0 }, { 0, 0, Color_Purple, 0 },
		},
		{
			{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, Color_Purple, Color_Purple, Color_Purple }, { 0, 0, Color_Purple, 0 },
		},
		{
			{ 0, 0, 0, 0 }, { 0, Color_Purple, 0, 0 }, { 0, Color_Purple, Color_Purple, 0 }, { 0, Color_Purple, 0, 0 },
		},
	},
	{
		{
			{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { Color_Red, Color_Red, 0, 0 }, { 0, Color_Red, Color_Red, 0 },
		},
		{
			{ 0, 0, 0, 0 }, { 0, 0, Color_Red, 0 }, { 0, Color_Red, Color_Red, 0 }, { 0, Color_Red, 0, 0 },
		},
		{
			{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { Color_Red, Color_Red, 0, 0 }, { 0, Color_Red, Color_Red, 0 },
		},
		{
			{ 0, 0, 0, 0 }, { 0, 0, Color_Red, 0 }, { 0, Color_Red, Color_Red, 0 }, { 0, Color_Red, 0, 0 },
		},
	},
};

static int		find_any_in_column (unsigned char arr[static 4][4], int column) {
	for (int i = 0; i < 4; i += 1) {
		if (arr[i][column] != Color_Empty) {
			return (1);
		}
	}
	return (0);
}

static int		find_any_in_row (unsigned char arr[static 4][4], int row) {
	for (int i = 0; i < 4; i += 1) {
		if (arr[row][i] != Color_Empty) {
			return (1);
		}
	}
	return (0);
}

static int		is_holding_collide (struct game *game) {
	for (int y = 0; y < Array_Count (game->holding); y += 1) {
		if (y + game->holding_y < 0) {
			continue;
		}
		if (y + game->holding_y >= Array_Count (game->field)) {
			break ;
		}
		for (int x = 0; x < Array_Count (game->holding[0]); x += 1) {
			if (x + game->holding_x < 0) {
				continue ;
			}
			if (x + game->holding_x >= Array_Count (game->field[0])) {
				break ;
			}
			if (game->holding[y][x] != Color_Empty) {
				if (game->field[y + game->holding_y][x + game->holding_x] != Color_Empty) {
					return (1);
				}
			}
		}
	}
	return (0);
}

static int		find_left_pad (struct game *game) {
	int		left_pad = 0;

	for (int i = 0; i < Array_Count (game->holding[0]) && !find_any_in_column (game->holding, i); i += 1) {
		left_pad += 1;
	}
	return (left_pad);
}

static int		can_move_left (struct game *game) {
	int		left_pad;
	int		result;

	left_pad = find_left_pad (game);
	result = game->holding_x + left_pad > 0;
	if (result) {
		game->holding_x -= 1;
		result = !is_holding_collide (game);
		game->holding_x += 1;
	}
	return (result);
}

static int		find_right_pad (struct game *game) {
	int		right_pad = 0;

	for (int i = 0; i < Array_Count (game->holding[0]) && !find_any_in_column (game->holding, Array_Count (game->holding[0]) - i - 1); i += 1) {
		right_pad += 1;
	}
	return (right_pad);
}

static int		can_move_right (struct game *game) {
	int		right_pad;
	int		result;

	right_pad = find_right_pad (game);
	result = game->holding_x - right_pad < Array_Count (game->field[0]) - Array_Count (game->holding[0]);
	if (result) {
		game->holding_x += 1;
		result = !is_holding_collide (game);
		game->holding_x -= 1;
	}
	return (result);
}

static int		find_down_pad (struct game *game) {
	int		down_pad = 0;

	for (int i = 0; i < Array_Count (game->holding) && !find_any_in_row (game->holding, Array_Count (game->holding) - i - 1); i += 1) {
		down_pad += 1;
	}
	return (down_pad);
}

static int		can_move_down (struct game *game) {
	int		down_pad;
	int		result;

	down_pad = find_down_pad (game);
	result = game->holding_y - down_pad < Array_Count (game->field) - Array_Count (game->holding);
	if (result) {
		game->holding_y += 1;
		result = !is_holding_collide (game);
		game->holding_y -= 1;
	}
	return (result);
}

static int		find_up_pad (struct game *game) {
	int		up_pad = 0;

	for (int i = 0; i < Array_Count (game->holding) && !find_any_in_row (game->holding, i); i += 1) {
		up_pad += 1;
	}
	return (up_pad);
}

static int		can_move_up (struct game *game) {
	int		up_pad;
	int		result;

	up_pad = find_up_pad (game);
	result = game->holding_y + up_pad > 0;
	if (result) {
		game->holding_y -= 1;
		result = !is_holding_collide (game);
		game->holding_y += 1;
	}
	return (result);
}

static void		print_holding (struct game *game) {
	for (int y = 0; y < Array_Count (game->holding); y += 1) {
		if (y + game->holding_y < 0) {
			continue;
		}
		if (y + game->holding_y >= Array_Count (game->field)) {
			break ;
		}
		for (int x = 0; x < Array_Count (game->holding[0]); x += 1) {
			if (x + game->holding_x < 0) {
				continue ;
			}
			if (x + game->holding_x >= Array_Count (game->field[0])) {
				break ;
			}
			if (game->holding[y][x] != Color_Empty) {
				game->field[y + game->holding_y][x + game->holding_x] = game->holding[y][x];
			}
		}
	}
}

static int	get_next_holding_index (void) {
	return (rand () % Array_Count (s_holdings));
}

static void		make_holding (struct game *game, int index) {
	game->holding_rotate = 0;
	memcpy (game->holding, s_holdings[index][0], sizeof game->holding);
	game->holding_x = Array_Count (game->field[0]) / 2 - Array_Count (game->holding[0]) / 2;
	game->holding_y = - Array_Count (game->holding);
}

static void		next_holding (struct game *game) {
	print_holding (game);
	game->current_holding_index = game->next_holding_index;
	game->next_holding_index = get_next_holding_index ();
	make_holding (game, game->current_holding_index);
}

static void		clear_full_rows (struct game *game) {
	for (int y = Array_Count (game->field) - 1; y >= 0; y -= 1) {
		int		full = 1;

		for (int x = 0; x < Array_Count (game->field[0]); x += 1) {
			if (game->field[y][x] == Color_Empty) {
				full = 0;
				break ;
			}
		}
		if (full) {
			memmove (game->field[1], game->field[0], game->field[y] - game->field[0]);
			memset (game->field[0], 0, sizeof game->field[0]);
			y += 1;
		}
	}
}

static void	clip_holding_rotate_index (struct game *game) {
	if (game->holding_rotate >= 4) {
		game->holding_rotate = 0;
	}
	if (game->holding_rotate < 0) {
		game->holding_rotate = 3;
	}
}

static void	rotate_holding (struct game *game, int dir) {
	int		holding_x_off = 0;
	int		holding_y_off = 0;

	game->holding_rotate += dir;
	clip_holding_rotate_index (game);
	memcpy (game->holding, s_holdings[game->current_holding_index][game->holding_rotate], sizeof game->holding);
	if (game->holding_x > Array_Count (game->holding[0]) / 2) {
		int		right_pad = find_right_pad (game);

		if (game->holding_x + (Array_Count (game->holding[0]) - right_pad) > Array_Count (game->field[0])) {
			holding_x_off = (game->holding_x + (Array_Count (game->holding[0]) - right_pad)) - Array_Count (game->field[0]);
			game->holding_x -= holding_x_off;
		}
	} else {
		int		left_pad = find_left_pad (game);

		if (game->holding_x + left_pad < 0) {
			holding_x_off = game->holding_x + left_pad;
			game->holding_x -= holding_x_off;
		}
	}
	if (game->holding_y > Array_Count (game->holding)) {
		int		down_pad = find_down_pad (game);

		if (game->holding_y + (Array_Count (game->holding) - down_pad) > Array_Count (game->field)) {
			holding_y_off = (game->holding_y + (Array_Count (game->holding) - down_pad)) - Array_Count (game->field);
			game->holding_y -= holding_y_off;
		}
	}
	if (is_holding_collide (game)) {
		int		ok = 0;

		if (can_move_left (game)) {
			game->holding_x -= 1;
			ok = !is_holding_collide (game);
			game->holding_x += !ok;
		}
		if (!ok && can_move_right (game)) {
			game->holding_x += 1;
			ok = !is_holding_collide (game);
			game->holding_x -= !ok;
		}
		if (!ok && can_move_up (game)) {
			game->holding_y -= 1;
			ok = !is_holding_collide (game);
			game->holding_y += !ok;
		}
		if (!ok) {
			game->holding_x += holding_x_off;
			game->holding_y += holding_y_off;
			game->holding_rotate -= dir;
			clip_holding_rotate_index (game);
			memcpy (game->holding, s_holdings[game->current_holding_index][game->holding_rotate], sizeof game->holding);
		}
	}
}

int		run_game (struct game *game, struct input *input) {
	if (!game->is_play) {
		if (input->is_play) {
			game->is_play = 1;
			init_game (game);
		} else {
			return (1);
		}
	}

	if (input->is_left && can_move_left (game)) {
		game->holding_x -= 1;
	}
	if (input->is_right && can_move_right (game)) {
		game->holding_x += 1;
	}
	if (input->is_sprint_down) {
		game->is_sprint = 1;
	}
	if (input->is_sprint_up) {
		game->is_sprint = 0;
	}
	if (input->is_rotate_left) {
		rotate_holding (game, -1);
	}
	if (input->is_rotate_right) {
		rotate_holding (game, 1);
	}

	size_t	time_move_elapsed = stm_since (game->time_last_move);
	if (stm_ms (time_move_elapsed) > (game->is_sprint ? game->sprint_freq : game->freq)) {
		game->time_last_move = stm_now ();

		if (can_move_down (game)) {
			game->holding_y += 1;
		} else if (game->holding_y <= -Array_Count (game->holding)) {
			printf ("gameover\n");
			game->is_play = 0;
		} else {
			next_holding (game);
			clear_full_rows (game);
		}
	}
	return (1);
}

void	init_game (struct game *game) {
	memset (game, 0, sizeof *game);

	srand(time(0));

	game->time_start = stm_now ();
	game->time_last_move = game->time_start;
	game->freq = 1000;
	game->sprint_freq = 100;

	game->is_play = 1;
	game->is_holding = 1;
	game->next_holding_index = get_next_holding_index ();
	game->current_holding_index = get_next_holding_index ();
	game->holding_rotate = 0;

	make_holding (game, game->current_holding_index);
}

