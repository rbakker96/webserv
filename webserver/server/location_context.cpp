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

location_context::location_context() : _root(), _allowed_method(0), _index(0), _autoindex(false) {}
location_context::~location_context(){}

void location_context::configure_location_context(string_iterator it, string_iterator end) {
    configure configure_array[5] = { &location_context::configure_root,
                                     &location_context::configure_allowed_method,
                                     &location_context::configure_autoindex,
                                     &location_context::configure_index,
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
    _autoindex = false;
}

int     location_context::identify_location_value(std::string str){
    if ((int)str.find("root") != -1)
        return root_;
    else if ((int)str.find("allowed_method") != -1)
        return method_;
    else if ((int)str.find("autoindex") != -1)
        return autoindex_;
    else if ((int)str.find("index") != -1)
        return index_;
    return unknown_;
}

#include <iostream>
void    location_context::configure_location(std::string str) {
    size_t start = str.find_first_of('/');
    size_t end = str.find_first_of(' ', start);

    _location = str.substr(start, end - start);
}

void    location_context::configure_root(std::string str){
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _root = str.substr(pos + 1);
}

void    location_context::configure_allowed_method(std::string str){
    size_t                      start = str.find_first_not_of(' ');
    size_t                      pos = str.find_first_of(' ', start);
    std::string                 tmp = str.substr(pos + 1);
    std::string                 value;

    while ((int)(pos =tmp.find_first_of(' ')) != -1) {
        value = tmp.substr(0, pos);
        _allowed_method.push_back(value);
        tmp = tmp.substr(pos + 1);
    }
    _allowed_method.push_back(tmp);
}

void    location_context::configure_index(std::string str){
    size_t                      start = str.find_first_not_of(' ');
    size_t                      pos = str.find_first_of(' ', start);
    std::string                 tmp = str.substr(pos + 1);
    std::string                 value;

    while ((int)(pos =tmp.find_first_of(' ')) != -1) {
        value = tmp.substr(0, pos);
        _index.push_back(value);
        tmp = tmp.substr(pos + 1);
    }
    _index.push_back(tmp);
}

void    location_context::configure_autoindex(std::string str){
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);
    std::string tmp = str.substr(pos + 1);

    if (tmp == "on")
        _autoindex = true;
    return;
}

void    location_context::invalid_element(std::string str) {
    if (str == "0")
        return;
    return;
}

//Getters
std::string                 location_context::get_location() {return _location;}
std::string                 location_context::get_root() {return _root;}
std::vector<std::string>    location_context::get_method() {return _allowed_method;}
std::vector<std::string>    location_context::get_index() {return _index;}
bool                        location_context::get_autoindex() {return _autoindex;}