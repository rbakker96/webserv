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

handler::handler(){}
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
		return 0; //need some error checking method
    return tmp;
}




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

std::string	handler::get_file(void)
{
	return (_file);
}

void		handler::clear_file(void)
{
	_file.clear();
}

