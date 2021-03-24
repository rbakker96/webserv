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
	std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	if (tcp.response(hello) == 1)
		return (1);
	return (0);
}
