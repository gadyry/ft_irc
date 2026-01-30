NAME =  ircserv
CPP = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98

SRC = main.cpp  Server.cpp Client.cpp ../commandes/PASS.cpp ../commandes/handleCmd
OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ) 
	$(CPP) $(CPPFLAGS) -o $(NAME) $(OBJ)

%.o: %.cpp  Server.hpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re