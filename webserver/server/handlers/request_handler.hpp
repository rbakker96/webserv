/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   request_handler.hpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/07 10:45:36 by roybakker     #+#    #+#                 */
/*   Updated: 2021/04/07 10:45:36 by roybakker     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_REQUEST_HANDLER_HPP
#define WEBSERV_REQUEST_HANDLER_HPP

//general includes
#include <map>

//custom includes
//#include "../server.hpp"
#include "handler.hpp"

class request_handler : public handler {
public:
    friend class server;

private:
    //Request headers
    std::string     _method;
    std::string     _location;
    std::string     _host;
    std::string     _user_agent;
    std::string     _accept_language;
    std::string     _authorization;

public:
    request_handler();
    ~request_handler();

    //Parse functions
    void    parse_request(int fd);

    int     open_requested_file(std::string location);
    void    clear_atributes();
};

#endif //WEBSERV_REQUEST_HANDLER_HPP
