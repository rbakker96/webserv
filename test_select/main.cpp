//
// test main for trying out select()
//

#include "../tcp_layer/tcp_connection.hpp"

int	calc_highest_fd(int fd_one, int fd_two)
{
	if (fd_one > fd_two)
		return (fd_one);
	return (fd_two);
}

int	accept_request(tcp_connection *connection)
{
	int	request_fd;

	std::cout << "activity on port 8080" << std::endl;
	request_fd = accept(connection->tcp_socket, (struct sockaddr *)&connection->addr, (socklen_t *)&connection->addr_len);
	fcntl(request_fd, F_SETFL, O_NONBLOCK);
	return (request_fd);
}

void	read_request(int request_fd)
{
	char	buff[30000];

	std::cout << "reading request..." << std::endl;
	int ret = read(request_fd, buff, 30000);
	std::cout << "read value " << ret << std::endl;
	std::cout << "--- BUFF_ONE ---" << std::endl;
	std::cout << buff << std::endl;
	std::cout << "--- BUFF_ONE ---" << std::endl;
}

int	open_requested_file(void)
{
	int	fd = -1;

	std::cout << "opening requested_file..." << std::endl;
	fd = open("../html_css_testfiles/test_one.html", O_RDONLY);
	std::cout << "FD " << fd << std::endl;
	fcntl(fd, F_SETFL, O_NONBLOCK);
	return (fd);
}

void	write_response(int request_fd, char *buffer)
{
	int		len = 0;

	std::cout << "writing response..." << std::endl;
	std::string	header1 = "HTTP/1.1 200 OK\n";
	std::string	header2 = "Content-Type: text/html; charset=UTF-8\n";
	std::string	header3 = "Content-Length: 313\n\n";
	while (buffer[len] != '\0')
		len++;
	write(request_fd, header1.c_str(), header1.length());
	write(request_fd, header2.c_str(), header2.length());
	write(request_fd, header3.c_str(), header3.length());
	write(request_fd, buffer, len);
}

int	main(void)
{
	fd_set			read_fds, write_fds;
	fd_set			save_read_fds, save_write_fds;
	tcp_connection	connection_one;
	int				highest_fd = -1, request_fd = -1, file_fd = -1;
	char			requested_file_buffer[30000] = {0};

	FD_ZERO(&read_fds);
	FD_ZERO(&write_fds);
	FD_ZERO(&save_read_fds);
	FD_ZERO(&save_write_fds);
	connection_one.create_socket();
	connection_one.bind_socket_address(8080);
	connection_one.create_connection(100);
	highest_fd = calc_highest_fd(highest_fd, connection_one.tcp_socket);
	while (1)
	{
		read_fds = save_read_fds;
		write_fds = save_write_fds;
		FD_SET(connection_one.tcp_socket, &read_fds);
		std::cout << "before select" << std::endl;
		int	select_value = select(highest_fd + 1, &read_fds, &write_fds, NULL, NULL);
		if (select_value == -1)
			std::cout << "select error" << std::endl;
		else if (select_value == 0)
			std::cout << "time out" << std::endl;
		else
		{
			if (FD_ISSET(connection_one.tcp_socket, &read_fds))
			{
				request_fd = accept_request(&connection_one);
				FD_SET(request_fd, &save_read_fds);
				highest_fd = calc_highest_fd(highest_fd, request_fd);
			}
			if (FD_ISSET(request_fd, &read_fds))
			{
				read_request(request_fd);
				FD_CLR(request_fd, &save_read_fds);
				FD_SET(request_fd, &save_write_fds);
				file_fd = open_requested_file();
				highest_fd = calc_highest_fd(highest_fd, file_fd);
				FD_SET(file_fd, &save_read_fds);
			}
			if (FD_ISSET(file_fd, &read_fds))
			{
				read(file_fd, requested_file_buffer, 30000);
				FD_CLR(file_fd, &save_read_fds);
			}
			if (FD_ISSET(request_fd, &write_fds))
			{
				write_response(request_fd, requested_file_buffer);
				FD_CLR(request_fd, &save_write_fds);
				close(request_fd);
				request_fd = -1;
			}
		}
	}
}
