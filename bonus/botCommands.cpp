# include "../includes/MovieBot.hpp"
# include "../includes/Server.hpp"

std::string	MovieBot::handleHelp()
{
	std::string helpText = "🎬 === MovieBot Commands === 🎬\r\n";
	helpText += "!quote - Random movie quote\r\n";
	helpText += "!quote <movie> - Quote from specific movie\r\n";
	helpText += "!suggest / !recommend - recommend movie\r\n";
	helpText += "!add <movie>|<quote> - Add a new quote\r\n";
	helpText += "!info <movie> - Movie information\r\n";
	helpText += "!help - This help message";

	return (helpText);
}

std::string	MovieBot::getRandomQuote()
{
	if (quotesByMovieId.empty())
		return ("No quotes available.");
	size_t r = std::rand() % quotesByMovieId.size();
	std::map<unsigned short, std::vector<t_Quote> >::iterator it = quotesByMovieId.begin();
	while (r--)
		++it;

	unsigned short movieId = it->first;
	std::vector<t_Quote>& vect = it->second;

	if (vect.empty())
		return ("No quotes available.");

	size_t index = std::rand() % vect.size();

	return ("🎬 " + moviesById[movieId].title + ": " + vect[index].text);
}

std::string	MovieBot::handleQuote(std::vector<std::string>& args)
{
	if (args.empty())
		return (getRandomQuote());

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
		return ("Movie not found: " + movieName);
	unsigned short	movieId = it->second;

	if (quotesByMovieId.find(movieId) == quotesByMovieId.end() || quotesByMovieId[movieId].empty())
		return ("No quotes available for this movie.");

	std::vector<t_Quote>& vec = quotesByMovieId[movieId];
	int index = std::rand() % vec.size();
	return ("🎬 " + moviesById[movieId].title + ": " + vec[index].text);
}

std::string	MovieBot::handleSuggest()
{
	if (moviesById.empty())
		return ("No movie available.");

	size_t	randSugg = rand() % moviesById.size();
	std::map<unsigned short, t_Movies>::iterator	it = moviesById.begin();

	while(randSugg--)
		it++;

	t_Movies& m = it->second;

	std::ostringstream oss;
	oss << "🎥 Recommendation: " << m.title;
	if (m.year > 0)
		oss << " (" << m.year << ")";
	if (!m.director.empty())
		oss << " - " << m.director;
	if (!m.rating.empty())
		oss << " | Rating: " << m.rating;

	return oss.str();
}

std::string	MovieBot::handleInfo(std::vector<std::string>& args)
{
	if (args.empty())
		return("Try, !info <movie>.");
	
	std::string	collMoviesName;
	int			movieId = -69;

	for(size_t i = 0; i < args.size(); i++)
	{
		collMoviesName += args[i];
		if (i < args.size() - 1)
			collMoviesName += " ";
	}

	std::map<std::string, unsigned short>::iterator	it;
	for(it = titleToId.begin(); it != titleToId.end(); it++)
	{
		if (it->first == collMoviesName)
		{
			movieId = it->second;
			break;
		}
	}
	if (movieId == -69)
		return ("Movie not found for now.");

	t_Movies& m = moviesById[movieId];

    std::ostringstream oss;
    oss << "🎬 **" << m.title << "** (" << m.year << ")\n"
        << "Directed by " << m.director << "\n"
        << "📖 " << m.plot;

    return (oss.str());
}

// std::string	MovieBot::handleAdd(std::vector<std::string>& args, std::string& sender)
// {
// 	// TODO
// }