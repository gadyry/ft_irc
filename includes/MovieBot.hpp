#ifndef MOVIEBOT_HPP
#define MOVIEBOT_HPP

# include "Client.hpp"
# include "IrcReplies.hpp"
# include "IrcCommon.hpp"


typedef struct	s_Movies
{
	uint16_t	id;
	std::string	title;
	uint16_t	years;
	std::string	rating;
	std::string	director;
	std::string	plot;

	// std::string	searchKey; // We can used for (lowercase title for fast lookup)
}	t_Movies;

typedef struct	s_Quote
{
	uint16_t	movieId;
	std::string	text;

	// std::string	addedBy;
	// std::time_t	timestamp;
}	t_Quote;


class MovieBot
{
private:
	// Attribut :
	int				socketBot;
	std::string     hostname;
	std::string     nick;
	std::string     user;
	unsigned short  servPort;
	std::string     password;
	std::string     recieveBuff;

	std::map<unsigned short, t_Movies> moviesById;
	std::map<std::string, unsigned short> titleToId;
	std::map<unsigned short, std::vector<t_Quote> > quotesByMovieId;

	// methods :
	void	connectToServer();
	void	sendPrivMsg(std::string& target, std::string& message);
	void	dealWithPrivMsg(std::string& prefix, std::vector<std::string>& args);
	void	processMsg(std::string& fullMsg);
	void	handleMessage();
	void	buildBot();

	// 8=> cmds methods
	std::string handleHelp();
	std::string	getRandomQuote();
	std::string handleQuote(std::vector<std::string>& args);
	std::string handleSuggest(std::vector<std::string>& args);
	std::string handleAdd(std::vector<std::string>& args, std::string& sender);
	std::string handleInfo(std::vector<std::string>& args);

	// leoading data from data base

	void	loadMovies(std::string db_movies);
	void	loadQuotes(std::string db_quotes);

public:
	// Constractor && Distractor:
		MovieBot();
		MovieBot(std::string host, u_short port, std::string password);
		~MovieBot();

	// Setters && Getters

	// methods helper
	void    executeMovieBot();
};

#endif