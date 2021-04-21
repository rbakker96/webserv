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
#include "../helper/helper.hpp"

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

void    location_context::configure_root(const std::string &str){_root = parse_string(str);}
void    location_context::configure_allowed_method(const std::string &str){_allowed_method = parse_vector(str);}
void    location_context::configure_index(const std::string &str){_index = parse_string(str);}
void    location_context::configure_ext(const std::string &str) {_ext = parse_vector(str);}
void    location_context::invalid_element(const std::string &str) {parse_invalid(str);}

void    location_context::configure_autoindex(const std::string &str){
	if (parse_string(str) == "on")
        _autoindex = true;
}



//Getters
std::string                 location_context::get_location_context() {return _location_context;}
std::string                 location_context::get_root() {return _root;}
std::string                 location_context::get_index() {return _index;}
std::vector<std::string>    location_context::get_method() {return _allowed_method;}
std::vector<std::string>    location_context::get_ext() {return _ext;}
bool                        location_context::get_autoindex() {return _autoindex;}