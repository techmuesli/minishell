# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alkane <alkane@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/02 17:22:40 by dbrandtn          #+#    #+#              #
#    Updated: 2022/04/05 14:38:37 by alkane           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell
CC		= gcc
CFLAGS	= -Wall -Werror -Wextra
LIB_LOC = ./libft
LIBRARY = libft.a
SRCS	= minishell.c data_structs.c
OBJS	= ${SRCS:.c=.o}

all: $(NAME)

$(LIBRARY):
	$(MAKE) -C $(LIB_LOC)

$(NAME): $(OBJS) $(LIBRARY)
	$(CC) $(CFLAGS) $(OBJS) -lreadline -o $(NAME) -L $(LIB_LOC) -lft

debug: $(CFLAGS) += -g3
debug: all

clean:
	rm -f $(OBJS)
	$(MAKE) clean -C $(LIB_LOC)

fclean: clean
	rm -f $(NAME)
	$(MAKE) fclean -C $(LIB_LOC)

re:	fclean all

.PHONY: all debug clean fclean re
