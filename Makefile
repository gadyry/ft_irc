NAME = ircserv
BONUS_NAME = MoviesBot
CPP = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98

SRC = src/main.cpp src/Server.cpp src/Client.cpp commandes/PASS.cpp commandes/handleCmd.cpp \
		commandes/NICK.cpp commandes/USER.cpp commandes/QUIT.cpp commandes/JOIN.cpp src/Channel.cpp \
		commandes/KICK.cpp commandes/MODE.cpp commandes/PRIVMSG.cpp commandes/INVITE.cpp commandes/TOPIC.cpp

BONUS_SRC = bonus/main.cpp bonus/MovieBot.cpp

OBJ = $(SRC:.cpp=.o)
BONUS_OBJ = $(BONUS_SRC:.cpp=.o)

all: $(NAME)

bonus: $(BONUS_NAME)

$(NAME): $(OBJ)
	$(CPP) $(CPPFLAGS) -o $(NAME) $(OBJ)

$(BONUS_NAME): $(BONUS_OBJ)
	$(CPP) $(CPPFLAGS) -o $(BONUS_NAME) $(BONUS_OBJ)

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(BONUS_OBJ)

fclean: clean
	rm -f $(NAME) $(BONUS_NAME)

re: fclean all

.PHONY: all bonus clean fclean re
