

NAME=tetris

DEPDIR=.dep
OBJDIR=.obj

SRC = main.c\
draw.c\
game.c
MSRC = sokol_unit.m
ALL_SRC= $(MSRC) $(SRC)

OBJ1=$(ALL_SRC:.c=.o)
OBJ2=$(OBJ1:.m=.o)
OBJ=$(addprefix $(OBJDIR)/,$(OBJ2))

CFLAGS += -Wall -Wextra -Werror -pedantic -Wno-sizeof-array-argument -Wno-unknown-warning-option -Wno-sizeof-pointer-div -Wno-unneeded-internal-declaration -g
MFLAGS += -g

CC = clang

all: $(OBJDIR) $(DEPDIR) $(NAME)

$(OBJDIR):
	mkdir $(OBJDIR)

$(DEPDIR):
	mkdir $(DEPDIR)


$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) -lGL -ldl -lm -lX11 -lasound -lXi -lXcursor
# 	OpenGL
# 	$(CC) -framework Cocoa -framework OpenGL $(OBJ) -o $(NAME)
# 	Metal
# 	$(CC) -framework Cocoa -framework Metal -framework MetalKit -framework QuartzCore $(OBJ) -o $(NAME) -I. -g

clean:
	rm -rf $(wildcard $(OBJDIR)/*.o)
	rm -rf $(wildcard $(DEPDIR)/*d)

fclean: clean
	rm -rf $(NAME)

re: fclean all


$(DEPDIR)/%.cd.compiled: %.c
	$(CC) $(CFLAGS) -MM $< | sed -E 's_[A-Za-z_]*\.o_obj/&_' > "$@.compiled"

$(DEPDIR)/%.md.compiled: %.m
	$(CC) $(CFLAGS) -MM $< | sed -E 's_[A-Za-z_]*\.o_obj/&_' > "$@.complied"

DEPFLAGS=-MT $@ -MMD -MP

$(OBJDIR)/%.o : %.c
$(OBJDIR)/%.o : %.c $(DEPDIR)/%.cd | .dep
	$(CC) -c $(CFLAGS) $(DEPFLAGS) -MF $(DEPDIR)/$*.cd.compiled $< -o $@
	mv -f $(DEPDIR)/$*.cd.compiled $(DEPDIR)/$*.cd
$(OBJDIR)/%.o : %.m
$(OBJDIR)/%.o : %.m $(DEPDIR)/%.md | .dep
	$(CC) -c $(MFLAGS) $(DEPFLAGS) -MF $(DEPDIR)/$*.md.compiled $< -o $@
	mv -f $(DEPDIR)/$*.md.compiled $(DEPDIR)/$*.md

$(DEPDIR)/%.md: ;
$(DEPDIR)/%.cd: ;
.PRECIOUS: $(DEPDIR)/%.md $(DEPDIR)/%.cd

include $(wildcard $(DEPDIR)/*.md) $(wildcard $(DEPDIR)/*.cd)







