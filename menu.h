
#ifndef Menu_H
#define Menu_H

enum menu_entry_type {
	Menu_Entry_button,
	Menu_Entry_label,
};

struct menu_entry {
	enum menu_entry_type	type;
	int						subtype;
	int						identity;
	int						level;
	union {
		struct {
			char	text[32];
		} button;
		struct {
			char	text[32];
		} label;
	};
};

struct menu {
	int					entries_count;
	int					entries_max;
	struct menu_entry	*entries;
};

int		add_menu_entry (struct menu *, const struct menu_entry *);
void	clear_menu (struct menu *);
void	free_menu_resources (struct menu *menu);

#endif /* Menu_H */
