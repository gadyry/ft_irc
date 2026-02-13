# include "../includes/MovieBot.hpp"
# include "../includes/Server.hpp"

std::string	MovieBot::handleHelp()
{
	std::string helpText = "🎬 **MovieBot Commands** 🎬\r\n";
	helpText += "!quote - Random movie quote\r\n";
	helpText += "!quote <movie> - Quote from specific movie\r\n";
	helpText += "!suggest / !recommend - recommend movie\r\n";
	helpText += "!add <movie>|<quote> - Add a new quote\r\n";
	helpText += "!info <movie> - Movie information\r\n";
	helpText += "!help - This help message";

	return (helpText);
}

std::string MovieBot::handleQuote(std::vector<std::string>& args)
{
	if (args.empty())
			return ("Usage: !quote <movie title>"); // for now I can deal with it by this way!!
	// if (args.empty())
	// 	return (getRandomQuote()); // TODO

	std::string movieName;
	for (size_t i = 0; i < args.size(); ++i)
	{
		movieName += args[i];
		if (i < args.size() - 1)
			movieName += " ";
	}

	for(size_t i = 0; i < movieName.length(); i++)
		movieName[i] = std::tolower(movieName[i]);

	std::map<std::string, unsigned short>::iterator it;
	it = titleToId.find(movieName);
	if (it == titleToId.end())
		return ("❌ Movie not found: " + movieName);
	unsigned short	movieId = it->second;

	if (quotesByMovieId.find(movieId) == quotesByMovieId.end() || quotesByMovieId[movieId].empty())
		return ("❌ No quotes available for this movie.");

	std::vector<t_Quote>& vec = quotesByMovieId[movieId];
	int index = std::rand() % vec.size();
	return ("🎬 " + moviesById[movieId].title + ": " + vec[index].text);
}

std::string	MovieBot::handleSuggest(std::vector<std::string>& args)
{
	// TODO
}

std::string	MovieBot::handleAdd(std::vector<std::string>& args, std::string& sender)
{
	// TODO
}


std::string	MovieBot::handleInfo(std::vector<std::string>& args)
{
	// TODO
}