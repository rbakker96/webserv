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

int	main(void)
{
	fd_set			read_fds, write_fds;
	fd_set			save_read_fds, save_write_fds;
	tcp_connection	connection_one, connection_two;
	int				highest_fd = -1;
	int				request_fd_one = -1, request_fd_two = -1;

	FD_ZERO(&read_fds);
	FD_ZERO(&write_fds);
	FD_ZERO(&save_read_fds);
	FD_ZERO(&save_write_fds);
	connection_one.create_socket();
	connection_one.bind_socket_address(8080);
	connection_one.create_connection(100);
	connection_two.create_socket();
	connection_two.bind_socket_address(9090);
	connection_two.create_connection(100);
	highest_fd = calc_highest_fd(connection_one.tcp_socket, connection_two.tcp_socket);
	while (1)
	{
		read_fds = save_read_fds;
		write_fds = save_write_fds;
		FD_SET(connection_one.tcp_socket, &read_fds);
		FD_SET(connection_two.tcp_socket, &read_fds);
		std::cout << "before select" << std::endl;
		int	select_value = select(highest_fd + 1, &read_fds, &write_fds, NULL, NULL);
		std::cout << strerror(errno) << std::endl;
		std::cout << "select done" << std::endl;
		if (select_value == -1)
			std::cout << "select error" << std::endl;
		else if (select_value == 0)
			std::cout << "time out" << std::endl;
		else
		{
			std::cout << "a" << std::endl;
			if (FD_ISSET(connection_one.tcp_socket, &read_fds))
			{
				std::cout << "activity on socket 8080" << std::endl;
				request_fd_one = accept(connection_one.tcp_socket, (struct sockaddr *)&connection_one.addr, (socklen_t *)&connection_one.addr_len);
				fcntl(request_fd_one, F_SETFL, O_NONBLOCK);
				FD_SET(request_fd_one, &save_read_fds);
				highest_fd = calc_highest_fd(highest_fd, request_fd_one);
			}
			if (FD_ISSET(connection_two.tcp_socket, &read_fds))
			{
				std::cout << "activity on socket 9090" << std::endl;
				request_fd_two = accept(connection_two.tcp_socket, (struct sockaddr *)&connection_two.addr, (socklen_t *)&connection_two.addr_len);
				fcntl(request_fd_two, F_SETFL, O_NONBLOCK);
				FD_SET(request_fd_two, &save_read_fds);
				highest_fd = calc_highest_fd(highest_fd, request_fd_two);
			}
			if (FD_ISSET(request_fd_one, &read_fds))
			{
				std::cout << "reading request from 8080" << std::endl;
				char	buff[30000];

				int ret = read(request_fd_one, buff, 30000);
				FD_CLR(request_fd_one, &save_read_fds);
				std::cout << "return value read: " << ret << std::endl;
				std::cout << strerror(errno) << std::endl;
				std::cout << "--- BUFF_ONE ---" << std::endl;
				std::cout << buff << std::endl;
				std::cout << "--- BUFF_ONE ---" << std::endl;
				FD_SET(request_fd_one, &save_write_fds);
			}
			if (FD_ISSET(request_fd_two, &read_fds))
			{
				std::cout << "reading request from 9090" << std::endl;
				char	buff[30000];

				int ret = read(request_fd_two, buff, 30000);
				FD_CLR(request_fd_two, &save_read_fds);
				std::cout << "return value read: " << ret << std::endl;
				std::cout << strerror(errno) << std::endl;
				std::cout << "--- BUFF_ONE ---" << std::endl;
				std::cout << buff << std::endl;
				std::cout << "--- BUFF_ONE ---" << std::endl;
				FD_SET(request_fd_two, &save_write_fds);
			}
			if (FD_ISSET(request_fd_one, &write_fds))
			{
				std::cout << "writing response to 8080" << std::endl;
				connection_one.write_file_content(request_fd_one, 1);
				FD_CLR(request_fd_one, &save_write_fds);
				close(request_fd_one);
				request_fd_one = -1;
			}
			if (FD_ISSET(request_fd_two, &write_fds))
			{
				std::cout << "writing response to 8080" << std::endl;
				connection_two.write_file_content(request_fd_two, 2);
				FD_CLR(request_fd_two, &save_write_fds);
				close(request_fd_two);
				request_fd_two = -1;
			}
		}
	}
}
