/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   webserver.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/30 16:28:34 by roybakker     #+#    #+#                 */
/*   Updated: 2021/05/03 17:19:43 by gbouwen       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_WEBSERVER_HPP
#define WEBSERV_WEBSERVER_HPP

//general includes
#include <fcntl.h>

//custom includes
#include "server.hpp"
#include "file_descriptors.hpp"

//custom color for better visibility
# define RESET			"\033[0m"
# define BLACK			"\033[30m"				/* Black */
# define RED			"\033[31m"				/* Red */
# define GREEN			"\033[32m"				/* Green */
# define YELLOW			"\033[33m"				/* Yellow */
# define BLUE			"\033[34m"				/* Blue */
# define MAGENTA		"\033[35m"				/* Magenta */
# define CYAN			"\033[36m"				/* Cyan */
# define WHITE			"\033[37m"				/* White */

class webserver {
public:
	typedef     std::vector<std::string>      			vector;
	typedef     std::vector<std::string>::iterator      vector_iterator;
    enum        webserver_values{ error_page_ = -2, unused_ = -1, ready_for_use_ = -1, valid_ = 7, invalid_ = 8, error_code_ = 399 };

private:
    std::vector<server>     _servers;

public:
    webserver();
    ~webserver();

    void    	load_configuration(char* config_file);
    void    	establish_connection();
    void    	run();

    //Debug-tool
    void        print_struct();
};

#endif //WEBSERV_WEBSERVER_HPP
