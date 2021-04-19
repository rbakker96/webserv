/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handler.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/07 13:10:33 by roybakker     #+#    #+#                 */
/*   Updated: 2021/04/07 13:10:33 by roybakker     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "handler.hpp"

#include <errno.h>

//--------------------OLD--------------------
void    handler::read_file(int fd) {
    char    buff[3000];
    int     ret = 1;

    while (ret > 0) {
        ret = read(fd, buff, 3000);
        _file.append(buff, ret);
        if (ret < 3000)
            break;
    }
    if (ret == -1) {
        return ; //need some error checking method
    }
    return;
}
//--------------------------------------------

handler::handler() : _content_length(0), _content_type(), _content_language(), _content_location(), _allow(), _file() {}
handler::~handler(){}

std::string    handler::read_request(int fd) {
    std::string tmp;
    char        buff[3000];
    int         ret;

    while ((ret = read(fd, buff, 3000)) > 0) {
        tmp.append(buff, ret);
        if (ret < 3000)
            break;
    }
    if (ret == -1)
		return NULL; //need some error checking method
    return tmp;
}


//Parse functions
void        handler::parse_content_length(const std::string &str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    std::string tmp = str.substr(pos + 1);
    _content_length = ft_atoi(tmp.c_str());
}

void        handler::parse_content_type(const std::string &str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _content_type = str.substr(pos + 1);
}

void        handler::parse_content_language(const std::string &str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _content_language = str.substr(pos + 1);
}

void        handler::parse_content_location(const std::string &str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _content_location = str.substr(pos + 1);
}

void        handler::parse_allow(const std::string &str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _allow = str.substr(pos + 1);
}

void        handler::invalid_argument(const std::string &str) {
    if (str == "0")
        return;
    return;
}


//Helper functions
void		handler::clear_file(void) {_file.clear();}


//Getter
int             handler::get_content_length() { return _content_length;}
std::string     handler::get_content_type() { return _content_type;}
std::string     handler::get_content_language() { return _content_language;}
std::string     handler::get_content_location() { return _content_location;}
std::string     handler::get_allow() { return _allow;}
std::string	    handler::get_file(void) { return (_file);}
