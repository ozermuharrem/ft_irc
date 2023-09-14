#include "../includes/cap.hpp"

// cap: protokol uzantılarını değiştirir;
// /cap ls

Cap::Cap(std::map<int, User> &users) : _users(users)
{}

Cap::~Cap() {}

void Cap::execute(std::vector<std::string> &arg, int fd)
{
	std::cout << ">CAP TEST<\n";
	(void)arg;
	User usr("", "", fd);
	this->_users.insert(std::make_pair(fd, usr));
}
