/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tcp_layer.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/16 16:27:11 by roybakker     #+#    #+#                 */
/*   Updated: 2021/03/16 16:27:11 by roybakker     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCP_CONNECTION_HPP
#define TCP_CONNECTION_HPP

#include <netinet/in.h>

class tcp_connection {
public:
    int server_fd;
    int tcp_socket;
    struct sockaddr_in addr;
    int addr_len;

    tcp_connection();
    tcp_connection(tcp_connection const &src);
    ~tcp_connection();

    tcp_connection & operator=(tcp_connection const &rhs);
};


#endif //WEBSERV_TCP_HPP
