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

webserver::webserver() : _servers(), _client_amount(0) {}
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
//            server._handler.set_index(_servers.size());
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
        int time_out = it->_time_out;
        if (port <= 0)
            throw std::invalid_argument("Error: invalid port in configuration file");
        if (time_out <= 0)
            throw std::invalid_argument("Error: invalid time out in configuration file");
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
        if (select(fd.get_max(), &fd.get_read(), &fd.get_write(), 0, 0) == -1) {
            std::cout << "ERNO = " << strerror(errno) << std::endl;
            throw std::runtime_error("Select failed");
        }
        for (size_t server_index = 0; server_index < _servers.size(); server_index++) {
            server *server = &_servers[server_index];

            //THIS IF STATEMENT ONLY USE FOR SOCKETS FROM SERVERS FOR NEW INCOMMING CONNECTIONS TO BE ADDED TO OPEN CONNECTION VECTOR

            if (fd.rdy_for_reading(server->get_tcp_socket())) //accept request
            {
                int newFD = accept(server->get_tcp_socket(), (struct sockaddr *) &server->_addr, (socklen_t *) &server->_addr_len);
                if (newFD == -1) {
                    std::cout << "ERNO = " << strerror(errno) << std::endl;
                    throw (std::runtime_error("Accept failed"));
                }
                fd.set_time_out(newFD); //THIS WOULDN'T WORK ANYMORE
                fcntl(newFD, F_SETFL, O_NONBLOCK);
                fd.accepted_request_update(newFD);
                server->_clients.push_back(client(newFD, _client_amount));
                _client_amount++;
            }

            // MAKE FOR LOOP TO LOOP THROUGH ALL OPEN FD -COULD BE VECTOR CONTAINER-
            for(size_t client_index = 0;  client_index < server->_clients.size(); client_index++) {
                client *client = &server->_clients[client_index];

                try {
                    if (fd.rdy_for_reading(client->_clientFD)) //handle requested file
                    {
                        std::string request_headers = read_browser_request(client->_clientFD); //IF NOTHING IS READ CLOSE?
                        if (!request_headers.empty())
                            fd.set_time_out(client->_clientFD);
                        if (server->update_request_buffer(client->_clientFD, request_headers) == valid_) {
                            client->_handler.parse_request(server->_request_buffer[client->_clientFD]);
                            server->remove_handled_request(client->_clientFD);
                            client->_fileFD = client->_handler.handle_request(server->_cgi_file_types, server->_location_blocks, server->get_error_page(), client->_index);
                            fd.handled_request_update(client->_fileFD, client->_clientFD, client->_handler.verify_content_type(), client->_handler.get_method());
                            if (server->_cgi_file_types.find(client->_handler.verify_content_type()) !=
                                std::string::npos) {
                                client->_cgi_inputFD = client->_handler.create_cgi_fd("input", client->_index);
                                fd.set_write_buffer(client->_cgi_inputFD);
                                fd.update_max(client->_cgi_inputFD);
                            }
                        }
                    }

                    if (fd.rdy_for_reading(client->_fileFD)) //read requested file
                    {
                        if (fd.rdy_for_writing(client->_fileFD) && client->_handler.get_status() < error_) {
                            if (server->_cgi_file_types.find(client->_handler.verify_content_type()) !=
                                std::string::npos &&
                                fd.rdy_for_writing(client->_cgi_inputFD))
                                client->_handler.execute_cgi(client->_cgi_inputFD, client->_fileFD,
                                                             server->_server_name, server->_port);
                            else
                                client->_handler.write_body_to_file(client->_fileFD);
                        }
                        if (client->_handler.get_status() != 204) {
                            if (client->_handler.verify_content_type() == "bla" &&
                                client->_handler.get_method() == "POST")
                                client->_handler.read_cgi_header_file(client->_fileFD,
                                                                      server->_request_buffer[client->_clientFD].get_body_size());
                            else
                                client->_handler.read_requested_file(client->_fileFD);
                        }
                        fd.read_request_update(client->_fileFD, client->_clientFD);
                        if (client->_cgi_inputFD != unused_)
                            fd.clr_from_write_buffer(client->_cgi_inputFD);
                    }

                    if (fd.rdy_for_writing(client->_clientFD)) //send response
                    {
                        client->_handler.send_response(client->_clientFD, client->_fileFD, server->_server_name);
                        fd.clr_from_write_buffer(client->_clientFD);
                        if (client->_cgi_inputFD != unused_)
                            close(client->_cgi_inputFD);
                        client->_cgi_inputFD = unused_;
                        close(client->_fileFD);
                        client->_fileFD = unused_;
                        fd.set_read_buffer(client->_clientFD);
                    }
                    fd.check_time_out(server->_clients, server->_time_out);

                } //TRY BLOCK

                catch (std::string &e) {
                    if (e.compare("Read browser request failed") == 0)
                    {
                        fd.clr_from_read_buffer(client->_clientFD);
                        server->remove_client(client->_clientFD);
                    }
                    else if (e.compare("Read requested file failed") == 0)
                    {
                        fd.clr_from_read_buffer(client->_fileFD);
                        close(client->_fileFD);
                        client->_fileFD = unused_;
                        server->remove_client(client->_clientFD);
                    }
                    else if (e.compare("Write body to file failed") == 0)
                    {
                        fd.clr_from_write_buffer(client->_fileFD);
                        fd.clr_from_read_buffer(client->_fileFD);
                        close(client->_fileFD);
                        client->_fileFD = unused_;
                        server->remove_client(client->_clientFD);
                    }
                } //CATCH BLOCK

            } //FOR LOOP OPENFDS

        } // FOR LOOP SERVERS

    } //WHILE(TRUE) LOOP

}//RUN FUNCTION


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
