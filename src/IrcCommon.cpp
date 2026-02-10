# include "../includes/IrcCommon.hpp"

void log(LogLevel level, const std::string &msg)
{
	switch (level)
	{
		case INFO:
			std::cout << C_GREEN << C_BOLD << "[SERVER] " << C_RESET << C_GREEN << msg << C_RESET << std::endl;
			break;
		case WARN:
			std::cout << C_YELLOW << "[WARN] " << C_RESET << C_YELLOW << msg << C_RESET << std::endl;
			break;
		case ERROR:
			std::cerr << C_RED << C_BOLD << "[ERROR] " << C_RESET << C_RED << msg << C_RESET << std::endl;
			break;
		case DEBUG:
			std::cout << C_BLUE << "[DEBUG] " << C_RESET << C_BLUE << msg << C_RESET << std::endl;
			break;
		case CLIENT:
			std::cout << C_MAGENTA << "[CLIENT] " << C_RESET << C_MAGENTA << msg << C_RESET << std::endl;
			break;
		case NEWCLIENT:
			std::cout << C_CYAN << "[NEW CLIENT] " << C_RESET << C_CYAN << msg << C_RESET << std::endl;
			break;
		case DISCONNECT:
			std::cout << C_MAGENTA << "[DISCONNECT] " << C_RESET << C_MAGENTA << msg << C_RESET << std::endl;
			break;
		case BOT:
			std::cout << C_GREEN << "[BOT] " << C_RESET << msg << std::endl;  // <-- added only
			break;
	}
}