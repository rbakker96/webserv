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



//custom includes
#include "handler.hpp"

class request_handler : public handler {
public:
    friend class server;
    typedef     void (request_handler::*parse)(const std::string &str);

private:
    //Request headers
    std::string     _method;
    std::string     _location;
    std::string     _protocol;
    std::string     _host;
    std::string     _user_agent;
    std::string     _accept_language;
    std::string     _authorization;
    std::string     _referer;
    std::string     _body;

public:
    request_handler();
    ~request_handler();

    //Parse functions
    void                        parse_request(std::vector<location_context> location, int fd, std::map<int, std::string> request_buffer);
    void                        parse_first_line(const std::string &str);
    void                        parse_host(const std::string &str);
    void                        parse_user_agent(const std::string &str);
    void                        parse_language(const std::string &str);
    void                        parse_authorization(const std::string &str);
    void                        parse_referer(const std::string &str);
    void                        parse_body(const std::string &str);
    int                         identify_request_value(const std::string &str);

    //Helper functions
    std::vector<std::string>    str_to_vector(std::string request);
    void                        configure_location(std::vector<location_context> location);
    int                         open_requested_file(std::string location);
    void                        clear_atributes();

    //Getter
    std::string                 get_method();
    std::string                 get_location();
    std::string                 get_protocol();
    std::string                 get_host();
    std::string                 get_user_agent();
    std::string                 get_accept_language();
    std::string                 get_authorization();
    std::string                 get_referer();
    std::string                 get_body();

    //Debug tool
    void                        print_request(); //DELETE LATER
};

#endif //WEBSERV_REQUEST_HANDLER_HPP
