/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   location_context.cpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/30 14:07:20 by roybakker     #+#    #+#                 */
/*   Updated: 2021/03/30 14:07:20 by roybakker     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "location_context.hpp"

location_context::location_context() : _location_context(), _root(), _index(), _allowed_method(0), _ext(0), _autoindex(false) {}
location_context::~location_context(){}

void location_context::configure_location_context(vector_iterator it, vector_iterator end) {
    configure configure_array[6] = { &location_context::configure_root,
                                     &location_context::configure_allowed_method,
                                     &location_context::configure_autoindex,
                                     &location_context::configure_index,
                                     &location_context::configure_ext,
                                     &location_context::invalid_element };

    clean_location_instance();
    configure_location(*it);
    for(; it != end; it++) {
        int config_id = identify_location_value(*it);
        configure function = configure_array[config_id];
        (this->*function)(*it);
    }
}

void    location_context::clean_location_instance() {
    _root.clear();
    _allowed_method.clear();
    _index.clear();
    _ext.clear();
    _autoindex = false;
}

int     location_context::identify_location_value(const std::string &str){
    if (str.find("root") != std::string::npos)
        return root_;
    else if (str.find("allowed_method") != std::string::npos)
        return method_;
    else if (str.find("autoindex") != std::string::npos)
        return autoindex_;
    else if (str.find("index") != std::string::npos)
        return index_;
    else if (str.find("ext") != std::string::npos)
        return ext_;
    return unknown_;
}

void    location_context::configure_location(const std::string &str) {
    size_t start = str.find_first_of('/');
    size_t end = str.find_first_of(' ', start);

	_location_context = str.substr(start, end - start);
}

void    location_context::configure_root(const std::string &str){
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _root = str.substr(pos + 1);
}

void    location_context::configure_allowed_method(const std::string &str){
    size_t                      start = str.find_first_not_of(' ');
    size_t                      pos = str.find_first_of(' ', start);
    std::string                 tmp = str.substr(pos + 1);
    std::string                 value;

    while ((pos =tmp.find_first_of(' ')) != std::string::npos) {
        value = tmp.substr(0, pos);
        _allowed_method.push_back(value);
        tmp = tmp.substr(pos + 1);
    }
    _allowed_method.push_back(tmp);
}

void    location_context::configure_index(const std::string &str){
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _index = str.substr(pos + 1);
}

void    location_context::configure_autoindex(const std::string &str){
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);
    std::string tmp = str.substr(pos + 1);

    if (tmp == "on")
        _autoindex = true;
}

void    location_context::configure_ext(const std::string &str) {
    size_t                      start = str.find_first_not_of(' ');
    size_t                      pos = str.find_first_of(' ', start);
    std::string                 tmp = str.substr(pos + 1);
    std::string                 value;

    while ((pos =tmp.find_first_of(' ')) != std::string::npos) {
        value = tmp.substr(0, pos);
        _ext.push_back(value);
        tmp = tmp.substr(pos + 1);
    }
    _ext.push_back(tmp);
}

void    location_context::invalid_element(const std::string &str) {
    if (str == "0")
        return;
}

//Getters
std::string                 location_context::get_location_context() {return _location_context;}
std::string                 location_context::get_root() {return _root;}
std::string                 location_context::get_index() {return _index;}
std::vector<std::string>    location_context::get_method() {return _allowed_method;}
std::vector<std::string>    location_context::get_ext() {return _ext;}
bool                        location_context::get_autoindex() {return _autoindex;}