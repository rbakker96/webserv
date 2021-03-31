/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/16 16:23:09 by roybakker     #+#    #+#                 */
/*   Updated: 2021/03/25 11:28:55 by gbouwen       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "tcp_layer/tcp_connection.hpp"

//int main() {
//
//    tcp_connection tcp;
//
//	if (tcp.create_socket() == 1)
//		return (1);
//	if (tcp.bind_socket_addrs(8080) == 1)
//		return (1);
//	if (tcp.create_connection(100) == 1)
//		return (1);
//	std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
//	if (tcp.response(hello) == 1)
//		return (1);
//	return (0);
//}

#include "server/webserver.hpp"
#include "server/server.hpp"
#include "server/parser_utils/configuration_parser.hpp"
#include "server/parser_utils/gnl/get_next_line.h"

void    start_webserver(char *config_file) {
    char *line;
    int ret;
    int fd = open(config_file, O_RDONLY);
    std::vector<std::string>    server_block;

    while ((ret = get_next_line(fd, &line)) == 1) {
        server_block.push_back(line);
        if (complete_server_block(server_block)) {
            std::vector<std::string>::iterator it = server_block.begin();
            for (; it != server_block.end(); it++) {
                printf("line = %hhd\n", *it->c_str());
            }
        }
        //free(line); not yet sure if needed?
    }
    //-> if ret == -1 reading error
    //-> if unclosed server block
    //throw ConfigError();
    close(fd);
}

int     main(int argc, char**argv) {
    webserver webserver;

    if (argc != 2) //only config file is allowed
        return -1;
    try {

        start_webserver(argv[1]);
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}
