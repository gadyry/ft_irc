# include "../includes/MovieBot.hpp"

MovieBot::MovieBot() : socketBot(-1), nick(""), user(""), servPort(prt),
                    password(password), buffRecieve("") { }

MovieBot::MovieBot(u_short port, std::string password) : socketBot(-1), nick(""), user(""), servPort(prt),
                    password(password), buffRecieve("") { }

MovieBot::~MovieBot()
{
    // manage the resources!!!
}


// methods 

void    Server::executeMovieBot()
{
    
}