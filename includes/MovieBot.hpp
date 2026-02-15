#ifndef MOVIEBOT_HPP
#define MOVIEBOT_HPP

# include "Client.hpp"
# include "IrcReplies.hpp"
# include "IrcCommon.hpp"


typedef struct	s_Movies
{
	unsigned short	id;
	std::string		title;
	unsigned short	year;
	std::string		rating;
	std::string		director;
	std::string		plot;

	// std::string	searchKey; // We can used for (lowercase title for fast lookup)
}	t_Movies;

typedef struct	s_Quote
{
	unsigned short	movieId;
	std::string		text;

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
	void	sendPrivMsg(std::string& target, std::string& message, const std::string& color);

	// IRC color codes and helpers for colored bot messages
	static const std::string IRC_COLOR_RESET;
	static const std::string IRC_COLOR_CYAN;
	static const std::string IRC_COLOR_GREEN;
	static const std::string IRC_COLOR_RED;
	static const std::string IRC_COLOR_YELLOW;
	static std::string colorize(const std::string &text, const std::string &color);
	void	dealWithPrivMsg(std::string& prefix, std::vector<std::string>& args);
	void	processMsg(std::string& fullMsg);
	void	handleMessage();
	void	buildBot();

	// 8=> cmds methods
	std::string handleHelp();
	std::string	getRandomQuote();
	std::string handleQuote(std::vector<std::string>& args);
	std::string handleSuggest();
	// std::string handleAdd(std::vector<std::string>& args, std::string& sender);
	std::string handleInfo(std::vector<std::string>& args);

	// loading data from data base

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