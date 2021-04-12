/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   webserver.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/30 16:30:47 by roybakker     #+#    #+#                 */
/*   Updated: 2021/03/30 16:30:47 by roybakker     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../gnl/get_next_line.hpp"
#include "webserver.hpp"
#include "server.hpp"

#include <iostream>
void webserver::print_struct() {
    std::cout << "Content of struct\n";

    for (std::vector<server>::iterator it = _servers.begin(); it != _servers.end(); it++) {
        server current = *it;

        std::cout << "------------- BEGIN SERVER BLOCK -------------\n";
        std::cout << "Port = " << current.get_port() << std::endl;
        std::cout << "Host = " << current.get_host() << std::endl;
        std::cout << "File size = " << current.get_file_size() << std::endl;
        std::cout << "Server name = " << current.get_server_name() << std::endl;
        std::cout << "Error page = " << current.get_error_page() << std::endl;

        std::vector<location_context> locations = current.get_location();
        for (std::vector<location_context>::iterator it = locations.begin(); it != locations.end(); it++) {
            location_context location = *it;

            std::cout << "\n  ------------- location block -------------\n";
            std::cout << "  Root = " << location.get_root() << std::endl;

            std::vector<std::string> methods = location.get_method();
            std::cout << "  Allowed methods = ";
            for (std::vector<std::string>::iterator it1 = methods.begin(); it1 != methods.end(); it1++) {
                std::string method = *it1;
                std::cout << method << " ";
            }
            std::cout << "\n";

            std::vector<std::string> indexen = location.get_index();
            std::cout << "  Index = ";
            for (std::vector<std::string>::iterator it2 = indexen.begin(); it2 != indexen.end(); it2++) {
                std::string index = *it2;
                std::cout << index << " ";
            }
            std::cout << "\n";
            std::cout << "  Autoindex = " << location.get_autoindex() << std::endl;
        }
        std::cout << "------------- END SERVER BLOCK -------------\n\n";
    }
}

webserver::webserver() : _servers(0), _highest_fd(-1), _request_fd(-1), _file_fd(-1) {}

webserver::~webserver(){}

void    webserver::load_configuration(char *config_file) {
    std::vector<std::string>    server_block;
    server                      server;
    char                        *line;
    int                         ret;
    int                         fd = open(config_file, O_RDONLY);

    while ((ret = get_next_line(fd, &line)) == 1) {
        server_block.push_back(line);
        if (check_server_block(server_block)) {
            server.create_new_server(server_block);
            _servers.push_back(server);
            server_block.clear();
        }
        //free(line); not yet sure if needed?
    }
    print_struct();                         //  PRINTING STRUCT
    close(fd);
//    if (ret == -1)
//        throw "Reading error occured";
//    if (!check_server_block(server_block))
//        throw "Syntax error in config file";
}

void    webserver::establish_connection(){
	int	index = 0;

    for (std::vector<server>::iterator it = _servers.begin(); it != _servers.end(); it++) {
        server current = *it;

        current.create_socket();
        current.bind_socket_address(current.get_port());
        current.create_connection(100); //CHECK LATER
		_servers[index] = current;
		index++;
    }
}

void    webserver::run() {
    initialize_fd_sets();
    initialize_highest_fd();
    while (1)
    {
        _read_fds = _buffer_read_fds;
        _write_fds = _buffer_write_fds;

        add_sockets_to_read_fds();
        std::cout << "before select" << std::endl;
        int	select_value = select(_highest_fd + 1, &_read_fds, &_write_fds, NULL, NULL);
        if (select_value == -1)
            std::cout << "select error" << std::endl;
        else if (select_value == 0)
            std::cout << "time out" << std::endl;
        else {
            accept_request();
            handle_request();
            read_request_file();
            create_response();
        }
    }
}

void    webserver::initialize_fd_sets() {
    FD_ZERO(&_read_fds);
    FD_ZERO(&_write_fds);
    FD_ZERO(&_buffer_read_fds);
    FD_ZERO(&_buffer_write_fds);
}

void    webserver::initialize_highest_fd() {
    std::vector<server>::iterator it = _servers.begin();
    server current = *it;

    _highest_fd = current.get_tcp_socket();
}

int		webserver::highest_fd(int fd_one, int fd_two) {
    if (fd_one > fd_two)
        return (fd_one);
    return (fd_two);
}

void    webserver::add_sockets_to_read_fds() {
    std::vector<server>::iterator it = _servers.begin();
    server current = *it;

    FD_SET(current.get_tcp_socket(), &_read_fds);
}

void    webserver::accept_request() {
    std::vector<server>::iterator it = _servers.begin();
    server current = *it;

    if (FD_ISSET(current.get_tcp_socket(), &_read_fds))
    {
        _request_fd = accept(current.get_tcp_socket(), (struct sockaddr *)&current._addr, (socklen_t *)&current._addr_len);
        fcntl(_request_fd, F_SETFL, O_NONBLOCK);
        FD_SET(_request_fd, &_buffer_read_fds);
        _highest_fd = highest_fd(_highest_fd, _request_fd);
    }
}

void    webserver::handle_request() {
    std::vector<server>::iterator it = _servers.begin();
    server current = *it;

    if (FD_ISSET(_request_fd, &_read_fds))
    {
        current._request.read_file(_request_fd);
        FD_CLR(_request_fd, &_buffer_read_fds);
        //some parsing functions needed to process request
		char location[500] = "/home/gijs/Desktop/codam/subjects/webserv/html_css_testfiles/test_one.html"; // temporary location for getting a file
        _file_fd = current._request.open_requested_file(location);
        _highest_fd = highest_fd(_highest_fd, _file_fd);
        FD_SET(_file_fd, &_buffer_read_fds);
    }
}

void    webserver::read_request_file() {
    std::vector<server>::iterator it = _servers.begin();
    server current = *it;

    if (FD_ISSET(_file_fd, &_read_fds))
    {
        current._request.read_file(_file_fd);
        FD_CLR(_file_fd, &_buffer_read_fds);
        FD_SET(_request_fd, &_buffer_write_fds);
    }
}

void    webserver::create_response() {
    std::vector<server>::iterator it = _servers.begin();
    server current = *it;

    if (FD_ISSET(_request_fd, &_write_fds))
    {
		std::cout << "response.get_file() " << current._response.get_file() << std::endl;
        current._response.create_response_file(_request_fd, current._response.get_file()); //see if compiles
        FD_CLR(_request_fd, &_buffer_write_fds);
        close(_request_fd);
        _request_fd = -1;
    }
}

int     webserver::check_server_block(std::vector <std::string> server_block) {
    int         open_bracket = 0;
    int         closing_bracket = 0;
    std::string str;

    for (std::vector<std::string>::iterator it = server_block.begin(); it != server_block.end(); it++) {
        str = it->data();
        if ((int)str.find("{") != -1)
            open_bracket++;
        if ((int)str.find("}") != -1)
            closing_bracket++;
    }
    if (open_bracket == closing_bracket && open_bracket != 0)
        return 1;
    return 0;
}
