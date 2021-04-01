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

#include "../../HEADER_FILES/CLASS/location_context.hpp"

location_context::location_context() : _root(0), _allowed_method(0), _index(0) {}
location_context::location_context(location_context const &src){*this = src;}
location_context::~location_context(){}

void location_context::configure_location_context(std::vector<std::string>::iterator it, std::vector<std::string>::iterator end) {
    std::string str;
    int config_id = 0;
    configure configure_array[5] = { &location_context::configure_root,
                                     &location_context::configure_allowed_method,
                                     &location_context::configure_index,
                                     &location_context::configure_autoindex,
                                     &location_context::invalid_element,};

    for(; it != end; it++) {
        str = it->data();
        if ((int)str.find("}") != -1)
            return;
        config_id = identify_location_value(*it);
        configure function = configure_array[config_id];
        (this->*function)(*it);
    }
}

int     location_context::identify_location_value(std::string str){
    if ((int)str.find("root") != -1)
        return 0;
    else if ((int)str.find("allowed_method") != -1)
        return 1;
    else if ((int)str.find("index") != -1)
        return 2;
    else if ((int)str.find("autoindex") != -1)
        return 3;
    return 4;
}

void    location_context::configure_root(std::string str){
    size_t pos = str.find_first_of(' ');
    _root = str.substr(pos + 1);
}

void    location_context::configure_allowed_method(std::string str){

}

void    location_context::configure_index(std::string str){

}

void    location_context::configure_autoindex(std::string str){
    size_t pos = str.find_first_of(' ');
    std::string tmp = str.substr(pos + 1);

    if (tmp == "on")
        _autoindex = true;
    _autoindex = false;
}

void    location_context::invalid_element(std::string str) {
    if (str == "0")
        return;
    return;
}