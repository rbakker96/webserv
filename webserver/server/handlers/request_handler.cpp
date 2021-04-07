/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   request_handler.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/07 10:45:44 by roybakker     #+#    #+#                 */
/*   Updated: 2021/04/07 10:45:44 by roybakker     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "request_handler.hpp"

request_handler::request_handler(){}
request_handler::~request_handler(){}

int     request_handler::open_requested_file(char *relative_path) {
    int	fd = open(relative_path, O_RDONLY);
    if (fd == -1)
        return -1; //need some error checking method
    fcntl(fd, F_SETFL, O_NONBLOCK);
    return (fd);
}

