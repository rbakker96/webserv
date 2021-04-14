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

//general includes
#include <string>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>

#include "../../helper/helper.hpp"

class handler {
private:
    //Entity headers
//    int             _content_length;
//    std::string     _content_type;
//    std::string     _conent_language;
//    std::string     _content_location;
//    std::string     _allow;

    std::string     _file; //local now?

    //Add map container with standard responses which can be altered in the actual response which will be send

public:
    handler();
    virtual ~handler();

    //Base class functions
    std::string     read_request(int fd);
    void            read_file(int fd); //old
	void		    clear_file(void);

    //GETTER
	std::string	    get_file(void);
};

#endif //WEBSERV_HANDLER_HPP
