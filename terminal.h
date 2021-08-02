
#ifndef Terminal_H
#define Terminal_H

struct terminal {
	char	term_desc[128];
	char	*clear;
};

void	init_terminal (struct terminal *);
void	clear_terminal (struct terminal *);

#endif /* Terminal_H */
