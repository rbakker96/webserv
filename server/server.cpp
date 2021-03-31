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

#include "server.hpp"

server::server();
server::server(location_context const &src);
server::~server();

server& server::operator=(server const &rhs);

void    server::create_new_server(std::vector <std::string> server_config) {
    for (std::vector<std::string>::iterator it; it != server_config.end(); it++) {
        std::string str = *it;
        if (str.find("port"))
            configure_port(str);
        else if (str.find("host"))
            configure_host(str);
        else if (str.find("server_name"))
            configure_server_name(str);
        else if (str.find("error_page"))
            configure_error_page(str);
        else if (str.find("max_file_size"))
            configure_max_file_size(str);
        else if (str.find("location")) {
            std::vector<std::string>::iterator begin = it;

            for (std::vector<std::string>::iterator end; end != server_config.end(); it++) {
                str = *end;
                if (str.find("}")) {
                    std::vector<std::string>    location_block(begin, end);
                    configure_location(location_block);
                    it += end;
                    break ;
                }
            }
        }
    }

}


void    server::configuration_table() {
    void *configuration_methods[6];
    configuration_methods[0] = configure_port();
    configuration_methods[1] = configure_host();
    configuration_methods[2] = configure_server_name();
    configuration_methods[3] = configure_error_page();
    configuration_methods[4] = configure_max_file_size();
    configuration_methods[5] = configure_location();
}


void    server::configure_location(std::vector <std::string> location_block) {

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

