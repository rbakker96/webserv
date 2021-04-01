/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/30 13:54:06 by roybakker     #+#    #+#                 */
/*   Updated: 2021/03/30 13:54:06 by roybakker     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../HEADER_FILES/CLASS/server.hpp"

server::server(){}
server::server(location_context const &src){*this = src;}
server::~server(){}

void    server::create_new_server(std::vector <std::string> server_config) {
    server  server;
    location_context location;
    int config_id = 0;
    configure configure_array[6] = { &server::configure_port,
                                     &server::configure_host,
                                     &server::configure_server_name,
                                     &server::configure_error_page,
                                     &server::configure_max_file_size,
                                     &server::invalid_element};

    for (std::vector<std::string>::iterator it = server_config.begin(); it != server_config.end(); it++) {
        config_id = identify_server_value(*it);
        if (config_id == 6) {
            location.configure_location_context(it, server_config.end());
            server._location.push_back(location);
        }
        configure function = configure_array[config_id];
        (this->*function)(*it);
    }
}

int     server::identify_server_value(std::string str) {
    if ((int)str.find("port") != -1)
        return 0;
    else if ((int)str.find("host") != -1)
        return 1;
    else if ((int)str.find("server_name") != -1)
        return 2;
    else if ((int)str.find("error_page") != -1)
        return 3;
    else if ((int)str.find("max_file_size") != -1)
        return 4;
    else if ((int)str.find("location") != -1)
        return 5;
    return 5;
}


void    server::configure_port(std::string str) {
    size_t pos = str.find_first_of(' ');
    std::string temp = str.substr(pos + 1);

    _port = atoi(temp.c_str());
}

void    server::configure_host(std::string str) {
    size_t pos = str.find_first_of(' ');
    _host = str.substr(pos + 1);
}

void    server::configure_server_name(std::string str) {
    size_t pos = str.find_first_of(' ');
    _server_name = str.substr(pos + 1);
}

void    server::configure_error_page(std::string str) {
    size_t pos = str.find_first_of(' ');
    _error_page = str.substr(pos + 1);
}

void    server::configure_max_file_size(std::string str) {
    size_t pos = str.find_first_of(' ');
    std::string temp = str.substr(pos + 1);

    _max_file_size = atoi(temp.c_str());
}

void    server::configure_location(std::vector <std::string> location_block) {
    if (location_block.size() == 0)
        return;
    return;
}

void    server::invalid_element(std::string str) {
    if (str == "0")
        return;
    return;
}

