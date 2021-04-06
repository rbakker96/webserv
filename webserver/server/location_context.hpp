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

//general includes
#include <string>
#include <vector>

class location_context {
public:
    typedef  void (location_context::*configure)(std::string);
    enum location_values{ root_ = 0, method_ = 1, autoindex_ = 2, index_ = 3, unknown_ = 4 };

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

    void    configure_location_context(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);
    int     identify_location_value(std::string str);

    void    configure_root(std::string str);
    void    configure_allowed_method(std::string str);
    void    configure_index(std::string str);
    void    configure_autoindex(std::string str);
    void    invalid_element(std::string str);

    int     context_size(std::vector<std::string>::iterator it);

    //GETTERS
    std::string                 get_root();
    std::vector<std::string>    get_method();
    std::vector<std::string>    get_index();
    bool                        get_autoindex();

};

#endif //WEBSERV_LOCATION_CONTEXT_H
