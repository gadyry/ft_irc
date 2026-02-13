# include "../includes/MovieBot.hpp"
# include "../includes/Server.hpp"

std::string	MovieBot::handleHelp()
{
	std::string helpText = "🎬 **MovieBot Commands** 🎬\r\n";
	helpText += "!quote - Random movie quote\r\n";
	helpText += "!quote <movie> - Quote from specific movie\r\n";
	helpText += "!list - List available movies\r\n";
	helpText += "!add <movie>|<quote> - Add a new quote\r\n";
	helpText += "!info <movie> - Movie information\r\n";
	helpText += "!help - This help message";
	
	return (helpText);
}