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

//before select
//GET / HTTP/1.1
//Host: localhost:8080
//Upgrade-Insecure-Requests: 1
//Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
//User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_6) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/14.0.3 Safari/605.1.15
//Accept-Language: en-us
//Accept-Encoding: gzip, deflate
//Connection: keep-alive
//
//GET /style.css HTTP/1.1
//Host: localhost:8080
//Connection: keep-alive
//Accept: text/css,*/*;q=0.1
//User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_6) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/14.0.3 Safari/605.1.15
//Accept-Language: en-us
//Referer: http://localhost:8080/
//Accept-Encoding: gzip, deflate


#include "request_handler.hpp"

request_handler::request_handler(){}
request_handler::~request_handler(){}

int     request_handler::open_requested_file(std::string location) {
	char	*path;
	int		fd;

	path = &location[0];
    fd = open(path, O_RDONLY);
    if (fd == -1)
        return -1; //need some error checking method
    fcntl(fd, F_SETFL, O_NONBLOCK);
    return (fd);
}

