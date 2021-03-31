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

#include "webserver.hpp"

webserver::webserver() : _servers(nullptr){}

webserver::webserver(webserver const &src){
    *this = src;
}

webserver::~webserver(){}

void    webserver::load_configuration(char *config_file) {
    char                        *line;
    int                         ret;
    int                         fd = open(argv[1], O_RDONLY);
    std::vector<std::string>    server_block;
    server                      server;

    while ((ret = get_next_line(fd, &line)) == 1) {
        server_block.push_back(line);
        if (check_server_block(server_block)) {
            server.create_new_server(server_block);
            _servers.push_back(server);
            server_block.clear();
        }
        //free(line); not yet sure if needed?
    }
    //-> if ret == -1 reading error
    //-> if unclosed server block
    //throw ConfigError();
    close(fd);
}

int     webserver::check_server_block(std::vector <std::string> str) {
    int     open_bracket = 0;
    int     closing_bracket = 0;

    for (std::vector<std::string>::iterator it; it != server_block.end(); it++) {
        std::string str = *it;
        if (str.find("{"))
            open_bracket++;
        else if (str.find("}"))
            closing_bracket++;
    }
    if (open_bracket == closing_bracket && open_bracket != 0)
        return 1;
    return 0;
}