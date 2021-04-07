/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response_handler.cpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/07 10:46:18 by roybakker     #+#    #+#                 */
/*   Updated: 2021/04/07 10:46:18 by roybakker     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "response_handler.hpp"

response_handler::response_handler(){}
response_handler::response_handler(response_handler const &src){}
response_handler::~response_handler(){}

void    response_handler::create_response_file(int fd, std::string response_file) {
    int		len = 0;

    std::cout << "writing response..." << std::endl;
    std::string	header1 = "HTTP/1.1 200 OK\n";
    std::string	header2 = "Content-Type: text/html; charset=UTF-8\n";
    std::string	header3 = "Content-Length: 313\n\n";
    while (buffer[len] != '\0')
        len++;
    write(request_fd, header1.c_str(), header1.length());
    write(request_fd, header2.c_str(), header2.length());
    write(request_fd, header3.c_str(), header3.length());
    write(request_fd, response_file.c_str(), len);
}