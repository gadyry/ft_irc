NAME = ircserv
CPP = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98

SRC = src/main.cpp src/Server.cpp src/Client.cpp commandes/PASS.cpp commandes/handleCmd.cpp \
		commandes/NICK.cpp commandes/USER.cpp commandes/QUIT.cpp commandes/JOIN.cpp src/Channel.cpp

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CPP) $(CPPFLAGS) -o $(NAME) $(OBJ)

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
