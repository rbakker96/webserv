/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   webserver.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/30 16:30:47 by roybakker     #+#    #+#                 */
/*   Updated: 2021/05/03 12:50:04 by gbouwen       ########   odam.nl         */
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

        std::vector<location_context> locations = current.get_location_blocks();
        for (std::vector<location_context>::iterator it = locations.begin(); it != locations.end(); it++) {
            location_context location = *it;

            std::cout << "\n  ------------- location block -------------\n";
            std::cout << "  location = " << location.get_location_context() << std::endl;
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
        }
        std::cout << "------------- END SERVER BLOCK -------------\n\n";
    }
}

webserver::webserver() : _servers(), _readFDS(), _writeFDS(), _buffer_readFDS(), _buffer_writeFDS(), _maxFD(-1) {}
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
	if (!server_block.empty()) // WHY
		throw std::invalid_argument("Error: missing '{' or '}' in config file");
    print_struct();                         //  PRINTING STRUCT
}

void    webserver::establish_connection(){
	for (size_t index = 0; index < _servers.size(); index++) {
        _servers[index].create_socket();
        _servers[index].bind_socket_address(_servers[index].get_port());
        _servers[index].create_connection(100); //CHECK LATER
	}
}

void    webserver::run() {
	initialize_FD_sets();
	initialize_handler_indexes();
    initialize_highest_fd();
    while (true)
    {
		synchronize_FD_sets();
		std::cout << "--- Waiting for activity... ---" << std::endl;
        if (select(get_maxFD(), &_readFDS, &_writeFDS, 0, 0) == -1)
		{
			std::cout << strerror(errno) << std::endl;
        	throw std::runtime_error("Select failed");
		}
        for (size_t index = 0; index < _servers.size(); index++) {
			server *server = &_servers[index];

			if (server->_activeFD == ready_for_use_ && FD_ISSET(server->get_tcp_socket(), &_readFDS)) //accept request
			{
				server->_activeFD = accept(server->get_tcp_socket(), (struct sockaddr *) &server->_addr, (socklen_t *) &server->_addr_len);
				fcntl(server->_activeFD, F_SETFL, O_NONBLOCK);
				FD_SET(server->_activeFD, &_buffer_readFDS);
				set_maxFD(server->_activeFD);
			}

			if (server->_activeFD != unused_ && FD_ISSET(server->_activeFD, &_readFDS)) //handle request
			{
				std::string request_headers = server->_handler.read_browser_request(server->_activeFD);
				if (server->update_request_buffer(server->_activeFD, request_headers) == valid_)
				{
					FD_CLR(server->_activeFD, &_buffer_readFDS);
					server->_handler.parse_request(server->_activeFD, server->_request_buffer);
                    server->clear_handled_request(server->_activeFD);
                    server->_fileFD = server->_handler.handle_request(server->_location_blocks, server->get_error_page(), server->get_file_size());

					set_maxFD(server->_fileFD);
					if (server->_fileFD != unused_)
					    FD_SET(server->_fileFD, &_buffer_readFDS);
					else
					    FD_SET(server->_activeFD, &_buffer_writeFDS);
                    if (server->_handler.verify_content_type() == "php" || server->_handler.get_method() == "PUT")
                        FD_SET(server->_fileFD, &_buffer_writeFDS); //_fileFD also needs to be added to buffer_writeFD in php cases or
				}
			}

			if (server->_fileFD != unused_ && FD_ISSET(server->_fileFD, &_readFDS)) //read requested file
			{
			    if (FD_ISSET(server->_fileFD, &_writeFDS)) {
                    if (server->_handler.verify_content_type() == "php")
			            server->_handler.execute_php(server->_fileFD);
                    else
                        server->_handler.write_put_file(server->_fileFD);
					FD_CLR(server->_fileFD, &_buffer_writeFDS);
			    }

			    if (server->_handler.get_status() != 204)
				    server->_handler.read_requested_file(server->_fileFD);
				FD_CLR(server->_fileFD, &_buffer_readFDS);
				FD_SET(server->_activeFD, &_buffer_writeFDS);
			}

			if (server->_activeFD != unused_ && FD_ISSET(server->_activeFD, &_writeFDS)) //create response
			{
				server->_handler.send_response(server->_activeFD, server->_fileFD, server->_server_name);
				close(server->_fileFD);
				server->_fileFD = unused_;
				FD_CLR(server->_activeFD, &_buffer_writeFDS);
				close(server->_activeFD);
				server->_activeFD = ready_for_use_;
			}
		}
    }
}

//------Helper functions------
void    webserver::synchronize_FD_sets() {
	_readFDS = _buffer_readFDS;
	_writeFDS = _buffer_writeFDS;

	for (size_t index = 0; index < _servers.size(); index++) {
		FD_SET(_servers[index].get_tcp_socket(), &_readFDS);
	}
}

void    webserver::initialize_FD_sets() {
    FD_ZERO(&_readFDS);
    FD_ZERO(&_writeFDS);
    FD_ZERO(&_buffer_readFDS);
    FD_ZERO(&_buffer_writeFDS);
}

void    webserver::initialize_highest_fd() {
	for (size_t index = 0; index < _servers.size(); index++) {
		if (_servers[index].get_tcp_socket() > _maxFD)
			_maxFD = _servers[index].get_tcp_socket();
	}
}

void	webserver::set_maxFD(int fd) {
	if (_maxFD > fd)
		return;
	_maxFD = fd;
}

int		webserver::get_maxFD() {
	return (_maxFD + 1);
}

int     webserver::check_server_block(webserver::vector server_block) {
	int         open_bracket = 0;
	int         closing_bracket = 0;
	std::string str;

	for (vector_iterator it = server_block.begin(); it != server_block.end(); it++) {
		str = *it;
		if (str.find('{') != std::string::npos)
			open_bracket++;
		if (str.find('}') != std::string::npos)
			closing_bracket++;
	}
	if (open_bracket == closing_bracket && open_bracket != 0)
		return 1;
	return 0;
}

void	webserver::initialize_handler_indexes()
{
	for (size_t index = 0; index < _servers.size(); index++) {
		_servers[index]._handler.set_index(index);
	}
}
