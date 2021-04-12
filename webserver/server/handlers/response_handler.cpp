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
response_handler::~response_handler(){}

void    response_handler::create_response_file(int request_fd, std::string response_file) {
    std::cout << "writing response..." << std::endl;
    std::string	header1 = "HTTP/1.1 200 OK\n";
    std::string	header2 = "Content-Type: text/html; charset=UTF-8\n";
    std::string	header3 = "Content-Length: 313\n\n";
	int	len = 0;
	while (response_file[len] != '\0')
		len++;

	std::cout << "LEN: " << len << std::endl;
    write(request_fd, header1.c_str(), header1.length());
    write(request_fd, header2.c_str(), header2.length());
    write(request_fd, header3.c_str(), header3.length());
    write(request_fd, response_file.c_str(), len);
}
