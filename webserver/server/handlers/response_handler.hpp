/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response_handler.hpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/07 10:46:09 by roybakker     #+#    #+#                 */
/*   Updated: 2021/04/07 10:46:09 by roybakker     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_RESPONSE_HANDLER_HPP
#define WEBSERV_RESPONSE_HANDLER_HPP

class response_handler {
private:


public:
    response_handler();
    response_handler(response_handler const &src);
    ~response_handler();

    void    create_response_file();
};


#endif //WEBSERV_RESPONSE_HANDLER_HPP
