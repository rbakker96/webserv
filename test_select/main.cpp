//
// test main for trying out select()
//

#include "../tcp_layer/tcp_connection.hpp"

int	main(void)
{
	tcp_connection	connection_one, connection_two;
	fd_set			read_fds;

	FD_ZERO(&read_fds);
	connection_one.create_socket();
	connection_one.bind_socket_address(8080);
	connection_one.create_connection(100);
	connection_two.create_socket();
	connection_two.bind_socket_address(9090);
	connection_two.create_connection(100);
	while (1)
	{
		FD_SET(connection_one.tcp_socket, &read_fds);
		FD_SET(connection_two.tcp_socket, &read_fds);
		std::cout << "1: " << connection_one.tcp_socket << std::endl;
		std::cout << "2: " << connection_two.tcp_socket << std::endl;
		int n = connection_two.tcp_socket + 1;
		std::cout << "before select" << std::endl;
		int	select_value = select(n, &read_fds, NULL, NULL, NULL);
		std::cout << "select done" << std::endl;
		if (select_value == -1)
			std::cout << "select error" << std::endl;
		else if (select_value == 0)
			std::cout << "time out" << std::endl;
		else
		{
			if (FD_ISSET(connection_one.tcp_socket, &read_fds))
			{
				int request_fd = accept(connection_one.tcp_socket, (struct sockaddr *)&connection_one.addr, (socklen_t *)&connection_one.addr_len);
				connection_one.write_file_content(request_fd, 1);
			}
			else
				std::cout << "connection one not set" << std::endl;
			if (FD_ISSET(connection_two.tcp_socket, &read_fds))
			{
				int request_fd = accept(connection_two.tcp_socket, (struct sockaddr *)&connection_two.addr, (socklen_t *)&connection_two.addr_len);
				connection_two.write_file_content(request_fd, 2);
			}
			else
				std::cout << "connection two not set" << std::endl;
		}
	}
}
