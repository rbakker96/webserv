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
    typedef     std::vector<std::string>::iterator vector_iterator;
    typedef     void (location_context::*configure)(const std::string&);
    enum        location_values{ root_ = 0, method_ = 1, autoindex_ = 2, index_ = 3, ext_ = 4, unknown_ = 5 };

private:
    std::string                 _location;
    std::string                 _root;
    std::string                 _index;
    std::vector<std::string>    _allowed_method;
    std::vector<std::string>    _ext;
    bool                        _autoindex;
    //something with the CGI

public:
    location_context();
    ~location_context();

    //Configure functions
    void    configure_location_context(vector_iterator begin, vector_iterator end);
    void    clean_location_instance();
    int     identify_location_value(const std::string &str);
    void    configure_location(const std::string &str);
    void    configure_root(const std::string &str);
    void    configure_allowed_method(const std::string &str);
    void    configure_index(const std::string &str);
    void    configure_autoindex(const std::string &str);
    void    configure_ext(const std::string &str);
    void    invalid_element(const std::string &str);

    //Getters
    std::string                 get_location();
    std::string                 get_root();
    std::string                 get_index();
    std::vector<std::string>    get_method();
    std::vector<std::string>    get_ext();
    bool                        get_autoindex();

};

#endif //WEBSERV_LOCATION_CONTEXT_H
