SRC_DIRS	:= channel/srcs irc_bots/srcs methods/srcs server/srcs user/srcs utils
INC_DIRS	:= channel/includes irc_bots/includes methods/includes server/includes user/includes utils
SRC_DIRS	+= main.cpp

NAME		= ircserver
CC			= c++
CFLAGS		= -Wall -Werror -Wextra -std=c++98
SRC			= $(shell find $(SRC_DIRS) -name '*.cpp')

all: $(NAME)

$(NAME): $(SRC)
	@echo "\033[31m• Creating •\033[0m"
	@$(CC) $(CFLAGS) $(SRC) -o $(NAME)
	@echo "\033[32m✓ Created ✓\033[0m"

clean:
	@rm -f $(NAME)
	@echo "\033[34m✓ Cleaned ✓\033[0m"

fclean: clean

re: clean all

.PHONY: all clean fclean re
