/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   webserver.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/30 16:30:47 by roybakker     #+#    #+#                 */
/*   Updated: 2021/04/15 12:18:27 by gbouwen       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../gnl/get_next_line.hpp"
#include "webserver.hpp"
#include "server.hpp"
#include <stdio.h> //REMOVE LATER!!!

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
            std::cout << "  location = " << location.get_location() << std::endl;
            std::cout << "  Root = " << location.get_root() << std::endl;

            std::vector<std::string> methods = location.get_method();
            std::cout << "  Allowed methods = ";
            for (std::vector<std::string>::iterator it1 = methods.begin(); it1 != methods.end(); it1++) {
                std::string method = *it1;
                std::cout << method << " ";
            }
            std::cout << "\n";

            std::cout << "  Index = " << location.get_index() << std::endl;
            std::cout << "  Autoindex = " << location.get_autoindex() << std::endl;

            std::vector<std::string> extensions = location.get_ext();
            std::cout << "  Ext = ";
            for (std::vector<std::string>::iterator it2 = extensions.begin(); it2 != extensions.end(); it2++) {
                std::string ext = *it2;
                std::cout << ext << " ";
            }
            std::cout << "\n";
        }
        std::cout << "------------- END SERVER BLOCK -------------\n\n";
    }
}

webserver::webserver() : _servers(0), _highest_fd(-1) {}
webserver::~webserver(){}

void    webserver::load_configuration(char *config_file) {
    std::vector<std::string>    server_block;
    server                      server;
    char                        *line;
    int                         ret;
    int                         fd = open(config_file, O_RDONLY);

	if (fd == -1)
		throw std::invalid_argument("Error opening config file");
    while ((ret = get_next_line(fd, &line)) == 1) {
        server_block.push_back(line);
        free(line);
        if (check_server_block(server_block)) {
            server.create_new_server(server_block);
            _servers.push_back(server);
            server_block.clear();
        }
    }
	free(line);
    close(fd);
	if (ret == -1)
		throw std::runtime_error("Error while reading config file");
	if (!server_block.empty())
		throw std::invalid_argument("Error: missing '{' or '}' in config file");
    print_struct();                         //  PRINTING STRUCT
}

int     webserver::check_server_block(std::vector <std::string> server_block) {
    int         open_bracket = 0;
    int         closing_bracket = 0;
    std::string str;

    for (std::vector<std::string>::iterator it = server_block.begin(); it != server_block.end(); it++) {
        str = *it;
        if (str.find('{') != std::string::npos)
            open_bracket++;
        if (str.find('}') != std::string::npos)
            closing_bracket++;
    }
    if (open_bracket == closing_bracket && open_bracket != 0) {
        return 1;
	}
    return 0;
}

void    webserver::establish_connection(){
	for (size_t index = 0; index < _servers.size(); index++) {
        _servers[index].create_socket();
        _servers[index].bind_socket_address(_servers[index].get_port());
        _servers[index].create_connection(100); //CHECK LATER
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
        if (select(_highest_fd + 1, &_read_fds, &_write_fds, NULL, NULL) == -1)
			throw std::runtime_error("Select failed");
        else {
            accept_request();
            handle_request();
            read_requested_file();
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
	for (size_t index = 0; index < _servers.size(); index++) {
		if (_servers[index].get_tcp_socket() > _highest_fd)
			_highest_fd = _servers[index].get_tcp_socket();
	}
}

int		webserver::highest_fd(int fd_one, int fd_two) {
    if (fd_one > fd_two)
        return (fd_one);
    return (fd_two);
}

void    webserver::add_sockets_to_read_fds() {
	for (size_t index = 0; index < _servers.size(); index++) {
	    FD_SET(_servers[index].get_tcp_socket(), &_read_fds);
	}
}

void    webserver::accept_request() {
	for (size_t index = 0; index < _servers.size(); index++)
	{
		if (FD_ISSET(_servers[index].get_tcp_socket(), &_read_fds))
		{
			_servers[index]._io_fd = accept(_servers[index].get_tcp_socket(), (struct sockaddr *)&_servers[index]._addr, (socklen_t *)&_servers[index]._addr_len);
			fcntl(_servers[index]._io_fd, F_SETFL, O_NONBLOCK);
			FD_SET(_servers[index]._io_fd, &_buffer_read_fds);
			_highest_fd = highest_fd(_highest_fd, _servers[index]._io_fd);
		}
	}
}

void    webserver::handle_request() {
    std::string     request_headers;
	std::string	    location;

	for (size_t index = 0; index < _servers.size(); index++) {
		if (_servers[index]._io_fd != -1 && FD_ISSET(_servers[index]._io_fd, &_read_fds))
		{
			request_headers = _servers[index]._handler.read_browser_request(_servers[index]._io_fd); //read
			int ret = _servers[index].update_request_buffer(_servers[index]._io_fd, request_headers);
			if (ret == valid_) {
				FD_CLR(_servers[index]._io_fd, &_buffer_read_fds);
                _servers[index]._handler.parse_request(_servers[index]._location, _servers[index]._io_fd, _servers[index]._request_buffer);
				_servers[index]._file_fd = _servers[index]._handler.open_requested_file(_servers[index]._handler.get_location());
				if (_servers[index]._file_fd == -1)
				{
					std::cout << "file not found" << std::endl;
					// get an error page / favicon / something else
				}
				_highest_fd = highest_fd(_highest_fd, _servers[index]._file_fd);
				_servers[index].clear_handled_request(_servers[index]._io_fd);
				FD_SET(_servers[index]._file_fd, &_buffer_read_fds);
			}
		}
	}
}

void    webserver::read_requested_file() {
	for (size_t index = 0; index < _servers.size(); index++) {
		if (_servers[index]._file_fd != -1 && FD_ISSET(_servers[index]._file_fd, &_read_fds))
		{
			_servers[index]._handler.read_requested_file(_servers[index]._file_fd);
			FD_CLR(_servers[index]._file_fd, &_buffer_read_fds);
			close(_servers[index]._file_fd);
			_servers[index]._file_fd = -1;
			FD_SET(_servers[index]._io_fd, &_buffer_write_fds);
		}
	}
}

void    webserver::create_response() {
	std::vector<std::string>	headers;

	for (size_t index = 0; index < _servers.size(); index++) {
		if (_servers[index]._io_fd != -1 && FD_ISSET(_servers[index]._io_fd, &_write_fds))
		{
			headers = _servers[index]._handler.create_response_headers();
			_servers[index]._handler.create_response_file(_servers[index]._io_fd, headers);
			FD_CLR(_servers[index]._io_fd, &_buffer_write_fds);
			close(_servers[index]._io_fd);
			_servers[index]._io_fd = -1;
			_servers[index]._handler.clear_requested_file();
		}
	}
}
