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
private:

public:
    request_handler();
    ~request_handler();

    int     open_requested_file(std::string location);
};


#endif //WEBSERV_REQUEST_HANDLER_HPP
