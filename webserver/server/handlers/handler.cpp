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

handler::handler(){}
handler::handler(handler const &src){}
handler::~handler(){}

void    handler::read_file(int fd) {
    char    buf[3000];
    int     ret = 1;

    while(ret > 0 ) {
        ret = read(file_fd, buff, 3000);
        if (location == response_)
            _file.append(buf, ret);
        if (ret < 3000)
            break;
    }
    if (ret == -1)
        return ; //need some error checking method
    return;
}