/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   webserver.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/30 16:28:34 by roybakker     #+#    #+#                 */
/*   Updated: 2021/03/30 16:28:34 by roybakker     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_WEBSERVER_HPP
#define WEBSERV_WEBSERVER_HPP

//general includes
#include <fcntl.h>

//custom includes
#include "server.hpp"

class webserver {
private:
    std::vector<server>     _servers;

public:
    webserver();
    webserver(webserver const &src);

    ~webserver();

    void    load_configuration(char* config_file);
    int     check_server_block(std::vector<std::string> server_block);

    //debug-tool
    void    print_struct();
};


#endif //WEBSERV_WEBSERVER_HPP
