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
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

tcp_connection::tcp_connection() {
	this->server_fd = 0;
	this->tcp_socket = 0;
	this->addr_len = 0;
}

//tcp_connection::tcp_connection(tcp_connection const &src){}

tcp_connection::~tcp_connection(){}

int tcp_connection::create_socket() {
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("cannot create tcp socket");
        return 1;
    }
    return 0;
}

int tcp_connection::bind_socket_addrs(int port) {
    memset((char *)&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if (bind(server_fd,(struct sockaddr *)&addr,sizeof(addr)) < 0) {
        perror("bind failed");
        return 1;
    }
    return 0;
}

int tcp_connection::create_connection(int backlog) {
    if (listen(server_fd, backlog) < 0) {
        perror("listen failed");
        return 1;
    }
    return 0;
}

int	tcp_connection::response(void) {
	while (1) {
		printf("LOOOOOOOOOOOOOOOOOOOOOOOOOOOP\n");
		std::cout << "\n+++++++ Waiting for new requests ++++++++\n\n" << std::endl;
        if ((this->tcp_socket = accept(this->server_fd, (struct sockaddr *)&this->addr, (socklen_t *)&this->addr_len)) < 0)
			return (1);
        char buffer[30000] = {0};
        int valread = read(this->tcp_socket , buffer, 30000);
		if (valread < 0)
			return (1);
		std::cout << buffer << std::endl;
		this->write_to_browser();
        std::cout << "------------------Response sent-------------------\n" << std::endl;
        close(this->tcp_socket);
    }
	return (0);
}

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>

void	tcp_connection::write_to_browser(void) {
	std::string	header1 = "HTTP/1.1 200 OK\n";
	std::string	header2 = "Content-Type: text/html; charset=UTF-8\n";
	std::string	header3 = "Content-Length: 302\n\n";
	int fd = open("html_css_testfiles/test.html", O_RDONLY);
	char buffer[30000] = {0};
	read(fd, buffer, 30000);
	int	len = 0;
	while (buffer[len] != '\0')
		len++;

	std::cout << len << std::endl;
	write(this->tcp_socket, header1.c_str(), header1.length());
	write(this->tcp_socket, header2.c_str(), header2.length());
	write(this->tcp_socket, header3.c_str(), header3.length());
	write(this->tcp_socket, buffer, len);
	std::string header4 = "Content-Type: text/ccs\n";
	std::string header5 = "Content-Length: 221\n\n";
	fd = open("html_css_testfiles/style.css", O_RDONLY);
	bzero(buffer, 30000);
	read(fd, buffer, 30000);
	len = 0;
	while (buffer[len] != '\0')
		len++;
	std::cout << buffer << std::endl;
	std::cout << len << std::endl;
	write(this->tcp_socket, header1.c_str(), header1.length());
	write(this->tcp_socket, header4.c_str(), header4.length());
	write(this->tcp_socket, header5.c_str(), header5.length());
	write(this->tcp_socket, buffer, len);
}
