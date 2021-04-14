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
#include <map>
#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <exception>
#include <fcntl.h>
#include <cstdlib> // REMOVE WITH ATOI REMOVAL

//custom includes
#include "location_context.hpp"
#include "handlers/handler.hpp"
#include "handlers/request_handler.hpp"
#include "handlers/response_handler.hpp"

//tcp-connection includes
#include <netinet/in.h>
#include <sys/socket.h>

class server {
public:
	friend class handler;
	friend class response_handler;
	friend class request_handler;
	friend class webserver;

public:
    typedef     std::vector<std::string>::iterator string_iterator;
    typedef     void (server::*configure)(std::string);
    enum        server_values{ port_ = 0, host_ = 1, server_name_ = 2, error_page_ = 3,
                                max_file_size_ = 4, unknown_ = 5, location_ = 6 };

private:
    //Connection
    int                             _io_fd;

    //Configurations
    int                             _max_file_size;
    int                             _port;
    std::string                     _host;
    std::string                     _server_name;
    std::string                     _error_page;
    std::vector<location_context>   _location;

    //TCP conection
    int 	                        _tcp_socket;
    int		                        _addr_len;
    struct	sockaddr_in             _addr;

    //Handlers
    std::map<int, std::string>      _request_buffer;
    request_handler                 _request;
    response_handler                _response;

public:
    server();
    ~server();

    //Configure functions
    void    clean_server_instance();
    int     identify_server_value(std::string str);
    void    create_new_server(std::vector<std::string> server_config);
    void    configure_port(std::string str);
    void    configure_host(std::string str);
    void    configure_server_name(std::string str);
    void    configure_max_file_size(std::string str);
    void    configure_error_page(std::string str);
    void    invalid_element(std::string str);

    //TCP-connection functions
    int     create_socket(void);
    int     bind_socket_address(int port);
    int     create_connection(int backlog);

    //Helper functions
    int     location_size(string_iterator it, string_iterator end);
    int     update_request_buffer(int fd, std::string request);
    int     valid_request(std::string);

    //Getters
    int                             get_file_size();
    int                             get_port();
    std::string                     get_host();
    std::string                     get_server_name();
    std::string                     get_error_page();
    std::vector<location_context>   get_location();
    int                             get_tcp_socket();
    int                             get_addr_len();
    struct	sockaddr_in             get_addr();
};

#endif //WEBSERV_SERVER_HPP
