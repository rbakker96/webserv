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

int	tcp_connection::response(std::string hello) {
	while (1) {
		std::cout << "\n+++++++ Waiting for new connection ++++++++\n\n" << std::endl;
        if ((this->tcp_socket = accept(this->server_fd, (struct sockaddr *)&this->addr, (socklen_t *)&this->addr_len)) < 0)
			return (1);
        char buffer[30000] = {0};
        int valread = read(this->tcp_socket , buffer, 30000);
		if (valread < 0)
			return (1);
		std::cout << buffer << std::endl;
        write(this->tcp_socket, hello.c_str(), hello.length());
        std::cout << "------------------Hello message sent-------------------\n" << std::endl;
        close(this->tcp_socket);
    }
	return (0);
}
