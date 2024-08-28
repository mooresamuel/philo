# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: samoore <samoore@student.42london.com>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/28 12:32:14 by samoore           #+#    #+#              #
#    Updated: 2024/08/28 13:54:30 by samoore          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_FILES = forks.c  init.c  philo.c  statics.c time.c print.c semaphore.c
OBJ_FILES = $(SRC_FILES:.c=.o)
NAME = philosophers

OBJ_DIR = obj

OBJ = $(addprefix $(OBJ_DIR)/, $(OBJ_FILES))
CC = gcc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) -lpthread

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ) 
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re