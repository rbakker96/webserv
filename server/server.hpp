/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/30 13:54:18 by roybakker     #+#    #+#                 */
/*   Updated: 2021/03/30 13:54:18 by roybakker     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

//general includes
#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <exception>

//custom includes
#include "location_context.hpp"

//tcp-connection includes
#include <netinet/in.h>
#include <sys/socket.h>

class server {
private:
    //Configurations
    int                             _max_file_size;
    int                             _port;
    std::string                     _host;
    std::string                     _server_name;
    std::string                     _error_page;
    std::vector<location_context>   _location;

    //TCP conection
    int 	                        _server_fd;
    int 	                        _tcp_socket;
    int		                        _addr_len;
    struct	sockaddr_in             _addr;

public:
    server();
    server(location_context const &src);
    ~server();

    server& operator=(server const &rhs);

    void    create_new_server(std::vector<std::string> server_config);
    void    configure_port(std::string str);
    void    configure_host(std::string str);
    void    configure_server_name(std::string str);
    void    configure_max_file_size(std::string str);
    void    configure_error_page(std::string str);
    void    configure_location(std::vector<std::string> location_block);

};

#endif //WEBSERV_SERVER_HPP
