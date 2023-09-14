#include "utils.hpp"

std::vector<std::string> parse(std::string str, std::string delimeter)
{
	std::vector<std::string>	ret;
	std::string					buffer;
	
	for (size_t i = 0; i < str.size(); i++)
	{
		buffer = str.substr(i, str.find_first_of(delimeter, i) - i);
		if (buffer.size() == 1 && buffer.find_first_of(" \r\n\t") != buffer.npos)
			(buffer.erase(buffer.find_first_of("\r\n\t ")));
		if (!buffer.empty())
		{
			ret.push_back(buffer);
			i += buffer.size();
		}
	}
	return ret;
}

bool channel_validate(std::map<std::string, Channel> channels, std::string channel_name, int fd)
{
	if (channel_name[0] != '#')
	{
		std::cerr << "Channel name has to start with #" << std::endl;
		send(fd, "Channel name has to start with #\r\n", 34, 0);
		return false;
	}
	std::map<std::string, Channel>::iterator it;
	for(it = channels.begin(); it != channels.end(); it++)
	{
		if(!strncmp(it->second.get_name().c_str(), channel_name.c_str(), it->second.get_name().size()))
			return false;
	}
	return true;
}

std::string trim(std::string& str, char delimeter)
{
    str.erase(str.find_last_not_of(delimeter)+1);
    str.erase(0, str.find_first_not_of(delimeter));
    return str;
}

void error(std::string msg, int fd)
{
	perror(msg.c_str());
	close(fd);
	exit(EXIT_FAILURE);
}
