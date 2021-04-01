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

#include "../../HEADER_FILES/UTILS/get_next_line.hpp"
#include "../../HEADER_FILES/CLASS/webserver.hpp"
#include "../../HEADER_FILES/CLASS/server.hpp"

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
            printf("XXXXXXXXXXXXXXX\n");
            for (std::vector<std::string>::iterator it = server_block.begin(); it != server_block.end(); it++) {
                std::cout << *it << std::endl;
            }
            printf("XXXXXXXXXXXXXXX\n\n");
            server.create_new_server(server_block);
            _servers.push_back(server);
            server_block.clear();
        }
        //free(line); not yet sure if needed?
    }
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