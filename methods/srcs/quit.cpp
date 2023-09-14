#include "../includes/quit.hpp"


Quit::Quit(std::map<int, User> &users, std::vector<pollfd> &fds, std::map<std::string, Channel> &channels) :  LeavingBot(users, channels, fds), _fds(fds), _users(users), _channels(channels){
	_message = new Privmsg(_users, _channels, fds);
}

Quit::~Quit() {
	delete this->_message;
}

void Quit::execute(std::vector<std::string> &arg, int fd)
{
	Part part(this->_channels);
    if (arg[1].size() > 160)
    {
        send(fd, "Quit message cannot exceed 160 characters\r\n", 43, 0);
        return ;
    }

    std::vector<pollfd>::iterator fd_it = this->_fds.begin();
    for(; fd_it->fd != fd; fd_it++) {}
	std::string reason(trim(arg.back(), ':') + "\r\n");
	this->send_message(fd, reason);
	
	std::map<int, User>::iterator _user_it = this->_users.find(fd);
    if (_user_it != this->_users.end())
	{
		std::vector<std::string> a;
		a.push_back("CLOSE");
		for (size_t i = 0; i < _user_it->second._channels.size(); i++)
		{
			a[1] = _user_it->second._channels[i];
			part.execute(a, fd);
		}
		this->_users.erase(_user_it);

	}
	this->_fds.erase(fd_it);

    close(fd);
}
