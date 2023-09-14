#include "../includes/pass.hpp"

// Yeni şifre belirler

Pass::Pass(std::map<int, User>& users, std::string pass) : _users(users), _pass(pass) {}

Pass::~Pass() {}

void Pass::execute(std::vector<std::string> &args, int fd)
{
	std::cout << ">PASS TEST<\n";
	for (size_t i = 0; i < args.size(); i++)
		std::cout << "---->ARG " << i << " " << args[i] << "<----\n";
	
	if (args.size() < 2)
	{
		std::string msg;
		msg.append(this->_users.find(-1)->second._prefix + " 461 " + this->_users.find(-1)->second._prefix + ": Insufficent parameters\r\n");
		send(fd, msg.c_str(), msg.size(), 0);
		return ;
	}
	std::map<int, User>::iterator it = this->_users.find(fd);
    if (this->_users.find(fd)->second._is_regis == 1)
	{
		std::string msg;
		msg.append(it->second._prefix + " 462 " + it->second._nickname + ": You are already registered1\r\n");
		send(fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	if (args[1].compare(this->_pass))
	{
		std::string msg;
		msg.append("Password wrong\r\n");
		send(fd, msg.c_str(), msg.size(), 0);
		close(fd);
		this->_users.find(fd)->second._joinable = -1;
		return ;
	}
	else if (!args[1].compare(this->_pass))
		this->_users.find(fd)->second._joinable = 1;
}
