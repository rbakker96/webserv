//
// test main for trying out select()
//

#include "../tcp_layer/tcp_connection.hpp"

int	main(void)
{
	tcp_connection	connection_one, connection_two;
	fd_set			read_fds;
	struct timeval	tv;
	char			buff_one[30000], buff_two[30000];
	int				n;

	FD_ZERO(&read_fds);
	connection_one.create_socket();
	connection_one.bind_socket_address(8080);
	connection_one.create_connection(100);
	connection_two.create_socket();
	connection_two.bind_socket_address(9090);
	connection_two.create_connection(100);
	tv.tv_sec = 10;
	tv.tv_usec = 0;
	while (1)
	{
		int	request_fd_one = accept(connection_one.tcp_socket, (struct sockaddr *)&connection_one.addr, (socklen_t *)&connection_one.addr_len);
		std::cout << "request_fd_one" << request_fd_one << std::endl;
		if (request_fd_one > 0)
		{
			FD_SET(request_fd_one, &read_fds);
			n = request_fd_one + 1;
		}
		int	request_fd_two = accept(connection_two.tcp_socket, (struct sockaddr *)&connection_two.addr, (socklen_t *)&connection_two.addr_len);
		std::cout << "request_fd_two" << request_fd_two << std::endl;
		if (request_fd_two > 0)
		{
			FD_SET(request_fd_two, &read_fds);
			n = request_fd_two + 1;
		}
		int	select_value = select(n, &read_fds, NULL, NULL, &tv);
		if (select_value == -1)
			std::cout << "select error" << std::endl;
		else if (select_value == 0)
			std::cout << "time out" << std::endl;
		else
		{
			if (FD_ISSET(request_fd_one, &read_fds))
			{
				recv(request_fd_one, buff_one, 30000, 0);
				std::cout << "--- Received in buff_one ---" << std::endl;
				std::cout << buff_one << std::endl;
				std::cout << "----------------------------" << std::endl;
				connection_one.write_file_content(request_fd_one, 1);
				FD_CLR(request_fd_one, &read_fds);
			}
			if (FD_ISSET(request_fd_two, &read_fds))
			{
				recv(request_fd_two, buff_two, 30000, 0);
				std::cout << "--- Received in buff_two ---" << std::endl;
				std::cout << buff_two << std::endl;
				std::cout << "----------------------------" << std::endl;
				connection_two.write_file_content(request_fd_two, 2);
				FD_CLR(request_fd_two, &read_fds);
			}
		}
	}
}
