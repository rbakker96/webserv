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

class handler {

private:
    std::string     _file;

public:
    handler();
    virtual ~handler();

    //Base class functions
    void    read_file(int fd);
	std::string	get_file(void);
};


#endif //WEBSERV_HANDLER_HPP
