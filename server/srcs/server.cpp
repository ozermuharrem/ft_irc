#include "../includes/server.hpp"

Server::Server(std::string arg, std::string pass) : port(atoi(arg.c_str())), listen_fd(-1), new_fd(-1),  _pass(pass) 
{
	User _filterbot("filter_bot", "irc", -2);
	this->users.insert(std::make_pair(-2, _filterbot)); // make_pair: 2 argümanı birleştirir
	User _file_transfer("file", "irc_file", -3);
	this->users.insert(std::make_pair(-3, _file_transfer));

	// IPv4 soketi için adres yapısının başlangıç değerlerini doldurur
	memset((char *)&this->addr, 0, sizeof(this->addr));
	this->addr.sin_family = AF_INET; // IPv4 adres ailesini temsil eder
	this->addr.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY: sunucu herhangi bir IP adresini kabul eder
	this->addr.sin_port = htons(this->port); // htons: host byte order'ındaki bir 16 bit değeri network byte order'a dönüştürür

	this->method["JOIN"] = new Join(this->users, this->channels);
	this->method["CAP"] = new Cap(this->users);
	this->method["QUIT"] = new Quit(this->users, this->fds, this->channels);
	this->method["NICK"] = new Nick(this->users);
	this->method["PASS"] = new Pass(this->users, this->_pass);
	this->method["USER"] = new Usercmd(this->users);
	this->method["PRIVMSG"] = new Privmsg(this->users, this->channels, this->fds);
	this->method["KICK"] = new Kick(*this);
	this->method["PING"] = new Ping(*this);
	this->method["PART"] = new Part(this->channels);
	this->method["MODE"] = new Mode(*this);

	this->create_socket();
	this->do_listen(this->listen_fd, 10);
}

Server::~Server()
{
	std::map<std::string, IMethod *>::iterator method_it = this->method.begin();
	for (; method_it != this->method.end(); method_it++)
		delete method_it->second;
}

void Server::create_socket()
{
	int optval = 1;

	this->listen_fd = socket(AF_INET, SOCK_STREAM, 0); // Soket oluşturur, SOCK_STREAM: güvenilir soket türü, 0: Standart protokol
	if (this->listen_fd < 0)
		error("socket () failed ", this->listen_fd);
	if (setsockopt(this->listen_fd, SOL_SOCKET, SO_REUSEADDR , &optval, sizeof(optval)) < 0) // Daha önce kullanılan adresi hızla yeniden kullanmayı sağlar
		error("setsockopt() failed ", this->listen_fd);
	if (setsockopt(this->listen_fd, IPPROTO_TCP, TCP_NODELAY , &optval, sizeof(optval)) < 0) // TCP bağlantısındaki gecikmeleri minimize eder
		error("setsockopt() failed ", this->listen_fd);
	if (fcntl(this->listen_fd, F_SETFL, O_NONBLOCK) < 0) // Soketi dosya işleminde(okuma/yazma/gönderme/alma) bloklanmayan modda çalışmasını sağlar. Böylece program akışı engellenmez
		error("fcnt() failed ", this->listen_fd);
	if (bind(this->listen_fd, (struct sockaddr *)&addr, sizeof(addr))) // Soketi IP adresine ve port numarasına bağlar
		error("bind() failed ", this->listen_fd);
}

void Server::do_listen(int fd, size_t listen_count)
{
	if (listen(fd, listen_count) < 0)
	{
		error("listen() failed ", this->listen_fd);
		return ;
	}
	std::cout << "Listen " << this->port << std::endl;
	this->fds.push_back((pollfd){fd, POLLIN, 0});
}

// Sunucuya gelen komutları işler
void Server::execute(std::string arg, int fd)
{
	std::vector<std::string> cmd = parse(arg, ":");
	std::vector<std::string> cmd2 = parse(cmd[0], " \r\t\n");
	for (size_t i = 1; i < cmd.size(); i++)
		cmd2.push_back(cmd[i]);
	std::transform(cmd2[0].begin(), cmd2[0].end(), cmd2[0].begin(), ::toupper);
	// cmd2[0] komut döner. Örn: JOIN
	// IMethod ile eşleşen komutu çalıştırır.
	std::map<std::string, IMethod *>::iterator it = this->method.find(cmd2[0]);
	if (it != this->method.end()){
		it->second->execute(cmd2, fd);}
}

int Server::get_user_on_server(std::string name)
{
	std::map<int, User>::iterator it = this->users.begin();

	for (; it != this->users.end(); it++)
	{
		if (!strncmp(it->second._nickname.c_str(), name.c_str(), it->second._nickname.size()))
			return it->second._fd;
	}
	return 0;
}

User& Server::get_user(int fd)
{
	return this->users.find(fd)->second;
}

Channel& Server::get_channel(std::string name)
{
	return this->channels.find(name)->second;
}

bool Server::search_channel(std::string name)
{
	if (this->channels.find(name) != this->channels.end())
		return 1;
	return 0;
}

void Server::do_recv(pollfd _fds)
{
	int rc = 1;
	char *buffer = new char[4096];
	memset(buffer, 0, 4096);
	rc = recv(_fds.fd, buffer, 4096, 0);
	std::cout << "buffer -> "<< buffer <<" <-" << std::endl;
	std::vector<std::string> temp = parse(buffer, "\r\n");
	for (size_t i = 0; i < temp.size(); i++)
		this->execute(temp[i], _fds.fd);
	if (rc <= 0)
	{
		std::cout << " Connection closed" << std::endl;
		this->users.erase(_fds.fd);
		std::vector<pollfd>::iterator it = this->fds.begin();
		for (; it->fd != _fds.fd; it++){}
		this->fds.erase(it);
		close(_fds.fd);
	}
	// kullanıcı kontrolü yapar: kullanıcı zaten kanala kayıtlı ise
	if (this->users.find(_fds.fd)->second._joinable == -1 && !this->users.find(_fds.fd)->second._nickname.empty())
	{
		std::string msg;
		msg.append(this->users.find(-1)->second._prefix + " 461 " + this->users.find(-1)->second._prefix + " Insufficent parameters\r\n");
		send(_fds.fd, msg.c_str(), msg.size(), 0);
		this->users.erase(_fds.fd);
		std::vector<pollfd>::iterator it = this->fds.begin();
		for (; it->fd != _fds.fd; it++){}
		this->fds.erase(it);
		close(_fds.fd);
	}
	delete[] buffer;
}

void Server::do_accept()
{
	// Bağlantı isteği geldiğinde, yeni bir dosya tanımlayıcısı oluşturulur ve bağlantı kabul edilir.
	// İlgili bağlantı noktası ve IP adresi bilgisi alınmaz (NULL olarak belirtilmiştir).
	this->new_fd = accept(this->listen_fd, NULL, NULL);

	std::string msg = "\nBağlantı Başarılı!\nIRC Server Komutları\n"
                  "- /cap\n"
                  "- /join\n"
                  "- /kick\n"
                  "- /mode\n"
                  "- /nick\n"
                  "- /part\n"
                  "- /pass\n"
                  "- /ping\n"
                  "- /privmsg\n"
                  "- /quit\n\n";
	send(new_fd, msg.c_str(), msg.size(), 0);

	this->fds.push_back((pollfd){new_fd, POLLIN, 0});
}

std::vector<pollfd> Server::get_fds() const {return this->fds;}
int	Server::get_listen_fd(){return this->listen_fd;}

void Server::erase_user(int fd)
{
	std::map<int, User>::iterator it = this->users.find(fd);
	if (it != this->users.end())
		this->users.erase(it);
}

void Server::erase_channel(std::string channel)
{
	std::map<std::string, Channel>::iterator it = this->channels.find(channel);
	if (it != this->channels.end())
		this->channels.erase(it);
}

void Server::print_users()
{
	std::map<int, User>::iterator it = this->users.begin();

	for(; it != this->users.end(); it++)
		std::cout << "INFO: " << it->second._nickname <<" | Connected fd: " << it->second._fd << " | IS regis: " << it->second._is_regis << std::endl;
}

std::string Server::get_pass()
{
	return this->_pass;
}
