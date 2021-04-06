/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tcp_layer.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/16 16:29:05 by roybakker     #+#    #+#                 */
/*   Updated: 2021/03/16 16:29:05 by roybakker     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "tcp_connection.hpp"

tcp_connection::tcp_connection() {
	this->tcp_socket = 0;
	this->addr_len = 0;
}

//tcp_connection::tcp_connection(tcp_connection const &src){}

tcp_connection::~tcp_connection(){}

int tcp_connection::create_socket() {
    if ((this->tcp_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "cannot create tcp socket" << std::endl;
        return 1;
    }
	fcntl(this->tcp_socket, F_SETFL, O_NONBLOCK);
    return 0;
}

int tcp_connection::bind_socket_address(int port) {
    memset((char *)&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if (bind(this->tcp_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		std::cout << "bind failed" << std::endl;
        return 1;
    }
    return 0;
}

int tcp_connection::create_connection(int backlog) {
    if (listen(this->tcp_socket, backlog) < 0) {
		std::cout << "listen failed" << std::endl;
        return 1;
    }
    return 0;
}

//int	tcp_connection::response(void) {
	//int	request_fd;

	//while (1) {
		//if ((request_fd = accept(this->tcp_socket, (struct sockaddr *)&this->addr, (socklen_t *)&this->addr_len)) < 0)
			//return (1);
		//return (request_fd);
		//char buffer[30000] = {0};
		//if (read(request_fd, buffer, 30000) < 0)
			//return (1);
		//handle_request(buffer, request_fd);
	//}
	//return (0);
//}

//void	tcp_connection::handle_request(char *buffer, int request_fd)
//{
////	check_request_method();
////	write_response_headers();
////	locate_and_read_file();
	//std::cout << "--- buffer ---" << std::endl;
	//std::cout << buffer << std::endl;
	//std::cout << "--- buffer ---" << std::endl;
	//write_file_content(request_fd);
	//close(request_fd);
//}

//void	tcp_connection::write_file_content(int request_fd) {
	//int		fd;
	//char	buffer[30000] = {0};

	//std::string	header1 = "HTTP/1.1 200 OK\n";
	//std::string	header2 = "Content-Type: text/html; charset=UTF-8\n";
	//std::string	header3 = "Content-Length: 313\n\n";
	//read(fd, buffer, 30000);
	//int	len = 0;
	//while (buffer[len] != '\0')
		//len++;

	//write(request_fd, header1.c_str(), header1.length());
	//write(request_fd, header2.c_str(), header2.length());
	//write(request_fd, header3.c_str(), header3.length());
	//write(request_fd, buffer, len);
//}
