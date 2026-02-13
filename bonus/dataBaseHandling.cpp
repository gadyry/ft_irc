# include "../includes/MovieBot.hpp"
# include "../includes/Server.hpp"

void	MovieBot::loadMovies(std::string db_movies)
{
	std::ifstream file(db_movies.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: Cannot open movies database: "
				  << db_movies << std::endl;
		return;
	}
	std::string	line;
	std::getline(file, line);

	while (std::getline(file, line))
	{
		if (line.empty())
			continue;
		
		std::stringstream	ss(line);
		std::string			token;
		t_Movies			movie;

		std::getline(ss, token, ',')
		movie.id = static_cast<uint16_t>(std::atoi(token.c_str()));

		std::getline(ss, movie.title, ',');

		std::getline(ss, token, ',');
		movie.years = static_cast<uint16_t>(std::atoi(token.c_str()));

		std::getline(ss, movie.rating, ',');
		std::getline(ss, movie.director, ',');
		std::getline(ss, movie.plot, ',');

		moviesById[movie.id] = movie;

		std::string lowerTitle = movie.title;
		for (size_t i = 0; i < lowerTitle.length(); ++i)
			lowerTitle[i] = std::tolower(lowerTitle[i]);
		
		titleToId[lowerTitle] = movie;
	}
}

void	MovieBot::loadQuotes(std::string db_quotes)
{
	// TODO
}

