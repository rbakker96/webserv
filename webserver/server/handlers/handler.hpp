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

//Custom includes
#include "../location_context.hpp"
#include "../../helper/helper.hpp"

class handler {
public:
    typedef     std::vector<std::string>::iterator      vector_iterator;
    typedef     std::map<int, std::string>::iterator    map_iterator;
    typedef     std::vector<location_context>::iterator location_iterator;
    enum        location_values{ host_ = 0, user_agent_ = 1, language_ = 2, authorization_ = 3, referer_ = 4, body_ = 5,
                                 content_length_ = 6, content_type_ = 7, content_language_ = 8, content_location_ = 9,
                                 allow_ = 10, unknown_ = 11};
protected:
    //Entity headers
    int             _content_length;
    std::string     _content_type;
    std::string     _content_language;
    std::string     _content_location;
    std::string     _allow;

    std::string     _file; //local now?

public:
    handler();
    virtual ~handler();

    //Base class functions
    std::string     read_request(int fd);
    void            read_file(int fd); //old

    //Parse functions
    void            parse_content_length(const std::string &str);
    void            parse_content_type(const std::string &str);
    void            parse_content_language(const std::string &str);
    void            parse_content_location(const std::string &str);
    void            parse_allow(const std::string &str);
    void            invalid_argument(const std::string &str);

    //Helper functions
    void		    clear_file();

    //GETTER
    int             get_content_length();
    std::string     get_content_type();
    std::string     get_content_language();
    std::string     get_content_location();
    std::string     get_allow();
	std::string	    get_file();
};

#endif //WEBSERV_HANDLER_HPP
