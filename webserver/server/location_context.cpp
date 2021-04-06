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
location_context::location_context(location_context const &src){*this = src;}
location_context::~location_context(){}

void location_context::configure_location_context(std::vector<std::string>::iterator it, std::vector<std::string>::iterator end) {
    std::string str;
    int config_id = 0;
    configure configure_array[5] = { &location_context::configure_root,
                                     &location_context::configure_allowed_method,
                                     &location_context::configure_autoindex,
                                     &location_context::configure_index,
                                     &location_context::invalid_element };

    for(; it != end; it++) {
        if (context_size(it))
            break;
        config_id = identify_location_value(*it);
        configure function = configure_array[config_id];
        (this->*function)(*it);
    }
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

void    location_context::configure_root(std::string str){
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _root = str.substr(pos + 1);
}

void    location_context::configure_allowed_method(std::string str){
    std::vector<std::string>    methods;
    size_t                      start = str.find_first_not_of(' ');
    size_t                      pos = str.find_first_of(' ', start);
    std::string                 tmp = str.substr(pos + 1);
    std::string                 value;

    while ((int)(pos =tmp.find_first_of(' ')) != -1) {
        value = tmp.substr(0, pos);
        methods.push_back(value);
        tmp = tmp.substr(pos + 1);
    }
    methods.push_back(tmp);
    _allowed_method = methods;
    methods.clear();
}

void    location_context::configure_index(std::string str){
    std::vector<std::string>    indexen;
    size_t                      start = str.find_first_not_of(' ');
    size_t                      pos = str.find_first_of(' ', start);
    std::string                 tmp = str.substr(pos + 1);
    std::string                 value;

    while ((int)(pos =tmp.find_first_of(' ')) != -1) {
        value = tmp.substr(0, pos);
        indexen.push_back(value);
        tmp = tmp.substr(pos + 1);
    }
    indexen.push_back(tmp);
    _index = indexen;
    indexen.clear();
}

void    location_context::configure_autoindex(std::string str){
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);
    std::string tmp = str.substr(pos + 1);

    if (tmp == "on")
        _autoindex = true;
    else
        _autoindex = false;
}

void    location_context::invalid_element(std::string str) {
    if (str == "0")
        return;
    return;
}

int     location_context::context_size(std::vector<std::string>::iterator it) {
    std::string str = it->data();

    if ((int)str.find("}") != -1)
        return 1;
    return 0;
}

//GETTERS
std::string                 location_context::get_root() {return _root;}
std::vector<std::string>    location_context::get_method() {return _allowed_method;}
std::vector<std::string>    location_context::get_index() {return _index;}
bool                        location_context::get_autoindex() {return _autoindex;}