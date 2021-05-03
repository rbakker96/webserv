/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/30 13:54:18 by roybakker     #+#    #+#                 */
/*   Updated: 2021/04/22 13:38:58 by gbouwen       ########   odam.nl         */
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
#include <unistd.h>
#include <exception>
#include <fcntl.h>

//custom includes
#include "location_context.hpp"
#include "header_handler.hpp"

//tcp-connection includes
#include <netinet/in.h>
#include <sys/socket.h>

class server {
public:
	friend class handler;
	friend class webserver;

public:
    typedef     std::vector<std::string>::iterator      vector_iterator;
    typedef     std::map<int, std::string>::iterator    map_iterator;
    typedef     void (server::*configure)(const std::string&);
    enum        server_values{ port_ = 0, host_ = 1, server_name_ = 2, error_page_ = 3,
                                max_file_size_ = 4, unknown_ = 5, location_ = 6, valid_ = 7, invalid_ = 8 };

private:
    //Connection
    int                             _activeFD;
	int								_fileFD;

    //Configurations
    int                             _max_file_size;
    int                             _port;
    std::string                     _host;
    std::string                     _server_name;
    std::string                     _error_page;
    std::vector<location_context>   _location_blocks;

    //TCP connection
    int 	                        _tcp_socket;
    int		                        _addr_len;
    struct	sockaddr_in             _addr;

    //Handler
    std::map<int, std::string>      _request_buffer;
    header_handler                  _handler;

public:
    server();
    ~server();

    //Configure functions
    void    clean_server_instance();
    int     identify_server_value(const std::string& str);
    void    create_new_server(std::vector<std::string> server_config);
    void    configure_port(const std::string& str);
    void    configure_host(const std::string& str);
    void    configure_server_name(const std::string& str);
    void    configure_max_file_size(const std::string& str);
    void    configure_error_page(const std::string& str);
    void    invalid_element(const std::string& str);

    //TCP-connection functions
    void	create_socket();
    void	bind_socket_address(int port);
    void	create_connection(int backlog);

    //Helper functions
    int     location_size(vector_iterator it, vector_iterator end);
    int     update_request_buffer(int fd, const std::string& request);
    void    clear_handled_request(int used_fd);
    int     valid_request(const std::string&);

    //Getters
    int                             get_file_size();
    int                             get_port();
    std::string                     get_host();
    std::string                     get_server_name();
    std::string                     get_error_page();
    std::vector<location_context>   get_location_blocks();
    int                             get_tcp_socket();
    int                             get_addr_len();
    struct	sockaddr_in             get_addr();
};

#endif //WEBSERV_SERVER_HPP
