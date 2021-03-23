/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/16 16:23:09 by roybakker     #+#    #+#                 */
/*   Updated: 2021/03/16 16:23:09 by roybakker     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "TCP_layer/tcp_connection.hpp"

int main() {
    tcp_connection tcp;

	if (tcp.create_socket() == 1)
		return (1);
	if (tcp.bind_socket_addrs(8080) == 1)
		return (1);
	if (tcp.create_connection(100) == 1)
		return (1);
    while (1) {
		std::cout << "\n+++++++ Waiting for new connection ++++++++\n\n" << std::endl;
        if ((tcp.tcp_socket = accept(tcp.server_fd, (struct sockaddr *)&tcp.addr, (socklen_t*)&tcp.addr_len)) < 0)
			return (1);
        char buffer[30000] = {0};
        int valread = read(tcp.tcp_socket , buffer, 30000);
		if (valread < 0)
			return (1);
		std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
		std::cout << buffer << std::endl;
        write(tcp.tcp_socket, hello.c_str(), hello.length());
        std::cout << "------------------Hello message sent-------------------\n" << std::endl;
        close(tcp.tcp_socket);
    }
	return (0);
}

