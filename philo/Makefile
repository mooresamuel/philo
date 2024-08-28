# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: samoore <samoore@student.42london.com>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/28 18:20:46 by samoore           #+#    #+#              #
#    Updated: 2024/08/28 18:26:04 by samoore          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_FILES = forks.c  init.c  locks.c  philo.c statics.c  time.c
OBJ_FILES = $(SRC_FILES:.c=.o)
OBJ_DIR = ./obj/
OBJ = $(addprefix $(OBJ_DIR), $(OBJ_FILES))
NAME = philo

CC = gcc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -g -lpthread

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJ): $(OBJ_DIR)%.o: %.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re