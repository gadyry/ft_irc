NAME = ircserv
BONUS_NAME = MoviesBot
CPP = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98

BONUS_SRC = bonus/main.cpp bonus/MovieBot.cpp bonus/botCommands.cpp bonus/dataBaseHandling.cpp src/IrcCommon.cpp
BONUS_OBJ = $(BONUS_SRC:.cpp=.o)

SRC = src/main.cpp src/Server.cpp src/Client.cpp commands/PASS.cpp commands/handleCmd.cpp \
		commands/NICK.cpp commands/USER.cpp commands/QUIT.cpp commands/JOIN.cpp src/Channel.cpp \
		commands/KICK.cpp commands/MODE.cpp commands/PRIVMSG.cpp commands/INVITE.cpp commands/TOPIC.cpp \
		src/IrcCommon.cpp
OBJ = $(SRC:.cpp=.o)

HDR = includes/Channel.hpp includes/Client.hpp includes/IrcCommon.hpp \
		includes/IrcReplies.hpp includes/Server.hpp
BONUS_HDR = includes/MovieBot.hpp includes/IrcCommon.hpp

RM = rm -rf

all : $(NAME)


$(NAME) : $(OBJ)
	$(CPP) $(CPPFLAGS) -o $(NAME) $(OBJ)

%.o : %.cpp $(HDR)
	$(CPP) $(CPPFLAGS) -c $< -o $@

$(BONUS_NAME) : $(BONUS_OBJ)
	$(CPP) $(CPPFLAGS) -o $(BONUS_NAME) $(BONUS_OBJ)

bonus/%.o : bonus/%.cpp $(BONUS_HDR)
	$(CPP) $(CPPFLAGS) -c $< -o $@

bonus : $(BONUS_NAME)

clean :
	$(RM) $(OBJ) $(BONUS_OBJ)

fclean : clean
	$(RM) $(NAME) $(BONUS_NAME)

re : fclean all