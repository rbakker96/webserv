/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   location_context.hpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/30 14:07:28 by roybakker     #+#    #+#                 */
/*   Updated: 2021/03/30 14:07:28 by roybakker     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_LOCATION_CONTEXT_H
#define WEBSERV_LOCATION_CONTEXT_H

#include <string>

class location_context {
private:
    std::string                 _root;
    std::vector<std::string>    _allowed_method;
    std::vector<std::string>    _index;
    bool                        _autoindex;
    //something with the CGI

public:
    location_context();
    location_context(location_context const &src);
    ~location_context();

    location_context& operator=(location_context const &rhs);
};

#endif //WEBSERV_LOCATION_CONTEXT_H
