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
	// if (args.empty())
	// 	return (getRandomQuote()); // TODO

	std::string movieName;
	for (size_t i = 0; i < args.size(); ++i)
	{
		if (i > 0)
			movieName += " ";
		movieName += args[i];
	}
	// return (searchQuoteByMovie(movieName)); // TODO
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