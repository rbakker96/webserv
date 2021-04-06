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
server::server(server const &src){*this = src;}
server::~server(){}

void    server::create_new_server(std::vector <std::string> server_config) {
    int                             config_id = 0;
    std::vector<location_context>   location_vector;
    location_context                location;
    configure configure_array[6] = { &server::configure_port,
                                     &server::configure_host,
                                     &server::configure_server_name,
                                     &server::configure_error_page,
                                     &server::configure_max_file_size,
                                     &server::invalid_element };

    for (std::vector<std::string>::iterator it = server_config.begin(); it != server_config.end(); it++) {
        config_id = identify_server_value(*it);
        if (config_id == location_) {
            location.configure_location_context(it, server_config.end());
            location_vector.push_back(location);
        }
        else {
            configure function = configure_array[config_id];
            (this->*function)(*it);
        }
    }
    _location = location_vector;
    location_vector.clear();
}

int     server::identify_server_value(std::string str) {
    if ((int)str.find("port") != -1)
        return port_;
    else if ((int)str.find("host") != -1)
        return host_;
    else if ((int)str.find("server_name") != -1)
        return server_name_;
    else if ((int)str.find("error_page") != -1)
        return error_page_;
    else if ((int)str.find("max_file_size") != -1)
        return max_file_size_;
    else if ((int)str.find("location") != -1)
        return location_;
    return unknown_;
}

void    server::configure_port(std::string str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);
    std::string temp = str.substr(pos + 1);

    _port = atoi(temp.c_str());
}

void    server::configure_host(std::string str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _host = str.substr(pos + 1);
}

void    server::configure_server_name(std::string str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _server_name = str.substr(pos + 1);
}

void    server::configure_error_page(std::string str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _error_page = str.substr(pos + 1);
}

void    server::configure_max_file_size(std::string str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);
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

//GETTERS
int                             server::get_file_size(){return _max_file_size;}
int                             server::get_port(){return _port;}
std::string                     server::get_host(){return _host;}
std::string                     server::get_server_name(){return _server_name;}
std::string                     server::get_error_page(){return _error_page;}
std::vector<location_context>   server::get_location(){return _location;}