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

webserver::webserver() : _servers(0){}

webserver::webserver(webserver const &src){
    *this = src;
}

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