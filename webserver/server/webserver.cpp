/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   webserver.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/30 16:30:47 by roybakker     #+#    #+#                 */
/*   Updated: 2021/05/18 17:14:29 by gbouwen       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../gnl/get_next_line.hpp"
#include "../helper/helper.hpp"
#include "webserver.hpp"
#include "server.hpp"

webserver::webserver() : _servers() {}
webserver::~webserver(){}


//-------------------------------------- GENERAL functions --------------------------------------
void    webserver::load_configuration(char *config_file) {
    std::vector<std::string>    server_block;
    server                      server;
    char                        *line = NULL;
    int                         ret;
    int                         fd = open(config_file, O_RDONLY);

	if (fd == -1)
		throw std::invalid_argument("Error opening config file");
    while ((ret = get_next_line(fd, &line)) == 1) {
        server_block.push_back(line);
        free(line);
        if (check_server_block(server_block)) {
            server.create_new_server(server_block);
            server._handler.set_index(_servers.size());
            _servers.push_back(server);
            server_block.clear();
        }
    }
    close(fd);
	if (line)
		free(line);
	if (ret == -1)
		throw std::runtime_error("Error while reading config file");
	if (!server_block.empty())
		throw std::runtime_error("Error: missing '{' or '}' in config file");
    print_struct();
}

void    webserver::validate_configuration() {
    for( std::vector<server>::iterator it = _servers.begin(); it != _servers.end(); it++) {
        bool duplicate = false;
        int port = it->_port;
        if (port <= 0)
            throw std::invalid_argument("Error: invalid port in configuration file");
        for(std::vector<server>::iterator compare = _servers.begin(); compare != _servers.end(); compare++) {
            if (port == compare->_port) {
                if (duplicate == true)
                    throw std::invalid_argument("Error: duplicate port in configuration file");
                duplicate = true;
            }
        }
    }
}

void    webserver::establish_connection(){
	for (size_t index = 0; index < _servers.size(); index++) {
        _servers[index].create_socket();
        _servers[index].bind_socket_address(_servers[index].get_port());
        _servers[index].create_connection(1000); //CHECK LATER
	}
}

void    webserver::run() {
    file_descriptors    fd;
    fd.initialize_max(_servers);
    std::cout << "--- Waiting for activity... ---" << std::endl;
    while (true)
    {
        fd.synchronize(_servers);
        if (select(fd.get_max(), &fd.get_read(), &fd.get_write(), 0, 0) == -1)
		{
			std::cout << RED << strerror(errno) << RESET << std::endl;
        	throw std::runtime_error("Select failed");
		}
        for (size_t index = 0; index < _servers.size(); index++) {
			server *server = &_servers[index];
            if (server->_activeFD == ready_for_use_ && fd.rdy_for_reading(server->get_tcp_socket())) //accept request
			{
				server->_activeFD = accept(server->get_tcp_socket(), (struct sockaddr *) &server->_addr, (socklen_t *) &server->_addr_len);
				if (server->_activeFD == -1)
					throw (std::runtime_error("Accept failed"));
				fd.set_time_out(server->_activeFD);
				fcntl(server->_activeFD, F_SETFL, O_NONBLOCK);
				fd.accepted_request_update(server->_activeFD);
			}

            if (fd.rdy_for_reading(server->_activeFD)) //handle requested file
			{
				std::string request_headers = read_browser_request(server->_activeFD);
                if (!request_headers.empty())
                    fd.set_time_out(server->_activeFD);
				if (server->update_request_buffer(server->_activeFD, request_headers) == valid_)
				{
					server->_handler.parse_request(server->_request_buffer[server->_activeFD]);
                    server->remove_handled_request(server->_activeFD);
                    server->_fileFD = server->_handler.handle_request(server->_cgi_file_types, server->_location_blocks, server->get_error_page());
                    fd.handled_request_update(server->_fileFD, server->_activeFD, server->_handler.verify_content_type(), server->_handler.get_method());
					if (server->_cgi_file_types.find(server->_handler.verify_content_type()) != std::string::npos)
					{
						server->_cgi_inputFD = server->_handler.create_cgi_fd("input");
						fd.set_write_buffer(server->_cgi_inputFD);
						fd.update_max(server->_cgi_inputFD);
					}
				}
			}

            if (fd.rdy_for_reading(server->_fileFD)) //read requested file
			{
				if (fd.rdy_for_writing(server->_fileFD) && server->_handler.get_status() < error_) {
                    if (server->_cgi_file_types.find(server->_handler.verify_content_type()) != std::string::npos && fd.rdy_for_writing(server->_cgi_inputFD))
						server->_handler.execute_cgi(server->_cgi_inputFD, server->_fileFD, server->_server_name, server->_port);
                    else
                        server->_handler.write_body_to_file(server->_fileFD);
			    }
			    if (server->_handler.get_status() != 204)
				    server->_handler.read_requested_file(server->_fileFD);
			    fd.read_request_update(server->_fileFD, server->_activeFD);
			}

			if (fd.rdy_for_writing(server->_activeFD)) //send response
			{
				server->_handler.send_response(server->_activeFD, server->_fileFD, server->_server_name);
                fd.clr_from_write_buffer(server->_activeFD);
				close(server->_activeFD);
                close(server->_fileFD);
                server->_activeFD = ready_for_use_;
                server->_fileFD = unused_;
			}
			if (server->_activeFD != ready_for_use_)
                server->_activeFD = fd.check_time_out(server->_activeFD, server->_time_out);
		}
    }
}


// // // // // // // // // // // // //  DEBUG functions // // // // // // // // // // // // //
#include <iostream>
void webserver::print_struct() {
    std::cout << "Content of struct\n";

    for (std::vector<server>::iterator it = _servers.begin(); it != _servers.end(); it++) {
        server current = *it;

        std::cout << "------------- BEGIN SERVER BLOCK -------------\n";
        std::cout << "Port = " << current.get_port() << std::endl;
        std::cout << "Host = " << current.get_host() << std::endl;
        std::cout << "Time out = " << current.get_time_out() << std::endl;
        std::cout << "Server name = " << current.get_server_name() << std::endl;
        std::cout << "Error page = " << current.get_error_page() << std::endl;
        std::cout << "CGI file types = " << current.get_cgi_file_types() << std::endl;

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
			std::cout << "  Redirect = " << location.get_redirect() << std::endl;
			std::cout << "  Max file size = " << location.get_max_file_size() << std::endl;
        }
        std::cout << "------------- END SERVER BLOCK -------------\n\n";
    }
}

void    webserver::print_fd_sets(file_descriptors fd) {
    for(int i = 0; i < fd.get_max(); i++) {
        if (fd.rdy_for_reading(i))
            std::cout << "RDY to read [" << i << "]" << std::endl;
        if (fd.rdy_for_writing(i))
            std::cout << "RDY to write [" << i << "]" << std::endl;
    }
}
