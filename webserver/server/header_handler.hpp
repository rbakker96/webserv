/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handler.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/07 13:23:53 by roybakker     #+#    #+#                 */
/*   Updated: 2021/04/07 13:23:53 by roybakker     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HANDLER_HPP
#define WEBSERV_HANDLER_HPP

//General includes
#include <map>
#include <string>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

//Custom includes
#include "location_context.hpp"
#include "../helper/helper.hpp"

class header_handler {
public:
    typedef     std::vector<std::string>                vector;
    typedef     std::vector<std::string>::iterator      vector_iterator;
    typedef     std::map<int, std::string>              map;
    typedef     std::map<int, std::string>::iterator    map_iterator;
    typedef     std::vector<location_context>           location_vector;
    typedef     std::vector<location_context>::iterator location_iterator;

    typedef     void (header_handler::*parse)(const std::string &str);

    enum        location_values{ requested_host_ = 0, user_agent_ = 1, language_ = 2, authorization_ = 3, referer_ = 4, body_ = 5,
                                 content_length_ = 6, content_type_ = 7, content_language_ = 8, content_location_ = 9,
                                 allow_ = 10, unknown_ = 11};

protected:
    //status
    int             _status;

    //Entity headers
    int             _content_length;
    std::string     _content_type;
    std::string     _content_language;
    std::string     _content_location;
    std::string     _allow;

    //Request headers
    std::string     _method;
    std::string     _file_location;
    std::string     _protocol;
    std::string     _requested_host;
    std::string     _user_agent;
    std::string     _accept_language;
    std::string     _authorization;
    std::string     _referer;
    std::string     _body;

    std::string     _requested_file;

public:
    header_handler();
    ~header_handler();

    //Parse request functions
    void            parse_request(location_vector location, int fd, map request_buffer);
    void            parse_first_line(const std::string &str);
    void            parse_requested_host(const std::string &str);
    void            parse_user_agent(const std::string &str);
    void            parse_language(const std::string &str);
    void            parse_authorization(const std::string &str);
    void            parse_referer(const std::string &str);
    void            parse_body(const std::string &str);
    int             identify_request_value(const std::string &str);

    void            parse_content_length(const std::string &str);
    void            parse_content_type(const std::string &str);
    void            parse_content_language(const std::string &str);
    void            parse_content_location(const std::string &str);
    void            parse_allow(const std::string &str);
    void            invalid_argument(const std::string &str);

    //Create response functions
	vector			create_response_headers(int io_fd);
	std::string		generate_response_code(void);
	std::string		generate_content_length(void);
	std::string		generate_content_type(void);
	std::string		generate_last_modified(int io_fd);

    void            create_response_file(int io_fd, vector headers);


    //Helper functions
    std::string     read_browser_request(int fd);
    void            read_requested_file(int fd);
    int             open_requested_file(std::string location);
    vector          str_to_vector(std::string request);
    void            configure_location(location_vector location_blocks);
    void		    clear_requested_file();
    void            clear_atributes();

    //Getter
    int             get_content_length();
    std::string     get_content_type();
    std::string     get_content_language();
    std::string     get_content_location();
    std::string     get_allow();
	std::string	    get_requested_file();
    std::string     get_method();
    std::string     get_file_location();
    std::string     get_protocol();
    std::string     get_requested_host();
    std::string     get_user_agent();
    std::string     get_accept_language();
    std::string     get_authorization();
    std::string     get_referer();
    std::string     get_body();

    //Debug tool
    void            print_request(); //DELETE LATER
};

#endif //WEBSERV_HANDLER_HPP
