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
public:
    typedef  void (server::*configure)(std::string);
    enum server_values{ port_ = 0, host_ = 1, server_name_ = 2, error_page_ = 3,
                        max_file_size_ = 4, unknown_ = 5, location_ = 6 };

private:
    //Configurations
    int                             _max_file_size;
    int                             _port;
    std::string                     _host;
    std::string                     _server_name;
    std::string                     _error_page;
    std::vector<location_context>   _location;

    //TCP conection
//    int 	                        _server_fd;
//    int 	                        _tcp_socket;
//    int		                        _addr_len;
//    struct	sockaddr_in             _addr;

public:
    server();
    server(server const &src);
    ~server();

    int     identify_server_value(std::string str);

    void    create_new_server(std::vector<std::string> server_config);
    void    configure_port(std::string str);
    void    configure_host(std::string str);
    void    configure_server_name(std::string str);
    void    configure_max_file_size(std::string str);
    void    configure_error_page(std::string str);
    void    configure_location(std::vector <std::string> location_block);
    void    invalid_element(std::string str);

    //GETTERS
    int                             get_file_size();
    int                             get_port();
    std::string                     get_host();
    std::string                     get_server_name();
    std::string                     get_error_page();
    std::vector<location_context>   get_location();
};

#endif //WEBSERV_SERVER_HPP
