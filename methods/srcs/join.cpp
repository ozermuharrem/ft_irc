#include "../includes/join.hpp"

Join::Join(std::map<int, User> &users, std::map<std::string, Channel> &channel) : _users(users), _channels(channel) {}

Join::~Join() {}

void Join::execute(std::vector<std::string> &arg, int fd)
{
	std::cout << ">JOIN TEST<\n";
	for (size_t i = 0; i < arg.size(); i++)
		std::cout << "---->ARG " << i << " " << arg[i] << "<----\n";

	if (arg[1][0] != '#' || !arg[1][1])
	{
		send(fd, "Channel name has to start with #\r\n", 34, 0);
		return;
	}

	// Kanalın aynı isminin benzersiz olup olmadığını kontrol eder
	if (channel_validate(this->_channels, arg[1], fd))
	{
		Channel cha(fd, arg[1], this->_users);
		
		cha.make_admin(fd);
		this->_channels.insert(std::make_pair(arg[1], cha));
	}

	// Kanalın doluluğunu kontrol eder
	if (this->_channels.find(arg[1])->second.get_limit() < (int)this->_channels.find(arg[1])->second.get_fds().size() + 1)
	{
		std::string msg;
		msg.append(this->_users.find(fd)->second._prefix + "471 " + this->_channels.find(arg[1])->second.get_name() + ": Channel is already full\r\n");
		send(fd, msg.c_str(), msg.size(), 0);
		return ;
	}
	
	// Kanalın şifre kontrolü
	if (!channel_validate(this->_channels, arg[1], fd))
	{
		if (!(this->_channels.find(arg[1])->second.get_pass().empty()))
		{
			if (arg.size() != 3 || this->_channels.find(arg[1])->second.get_pass() != arg[2])
			{
				std::string msg;
				msg.append(this->_users.find(fd)->second._prefix + "475 " + this->_users.find(fd)->second._nickname + ": Incorrect channel password\r\n");
				send(fd, msg.c_str(), msg.size(), 0);
				return ;
			}
		}
		std::map<std::string, Channel>::iterator channel_it = this->_channels.find(arg[1]);
		std::map<int, User>::iterator user_it = this->_users.find(fd);
		if (user_it != this->_users.end())
		{;
			if (channel_it != this->_channels.end())
			{
				channel_it->second.add_user(fd);
				user_it->second._channels.push_back(channel_it->second.get_name());
			}
			std::string a(user_it->second._prefix + "JOIN ");
			a.append(arg[1] + "\r\n");
			std::cout << a << std::endl;
			send(fd, a.c_str(), a.length(), 0);
		}
	}
}
