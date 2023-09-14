#include "../includes/filter_bot.hpp"

// yasaklı kelimeleri algılar

FilterBot::FilterBot(std::map<int, User> &users, std::map<std::string, Channel> &channels) : _users(users), _channels(channels)
{
	(void)_channels;
	this->_blacklist_words.push_back("at");
	this->_blacklist_words.push_back("sarkozy");
	this->_blacklist_words.push_back("dus perdesi");
	this->_blacklist_words.push_back("pis tuvalet terligi");
}

FilterBot::~FilterBot() {}


int	FilterBot::check_message(int fd, std::string msg)
{
	std::string _msg;

	std::transform(msg.begin(), msg.end(), msg.begin(), ::tolower);
	for (size_t i = 0; i < this->_blacklist_words.size(); i++)
	{
		if (msg.find(this->_blacklist_words[i]) != std::string::npos)
		{
			_msg.append(this->_users.find(-2)->second._prefix + "NOTICE " + this->_users.find(-2)->second._prefix + ": BLACKLISTED WORD WARNING. MESSAGE WASN'T SENT\r\n");
			send(fd, _msg.c_str(), _msg.size(), 0);
			return 0;
		}
	}
	return 1;
}
