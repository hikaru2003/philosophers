NAME = philo

SRCS	= action.c	\
		  ft_atoi.c	\
		  ft_init.c	\
		  main.c	\
		  philo.c	\

INCLUDE_DIR = include
SRCDIR = ./srcs/
OBJDIR = ./obj/

OBJS = $(SRCS:%.c=$(OBJDIR)%.o)

CC = cc

CFLAGS = -Wall -Wextra -Werror -I $(INCLUDE_DIR) #-g -fsanitize=address

all: $(OBJDIR) $(NAME)

$(NAME): $(OBJDIR) $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re