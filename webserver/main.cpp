/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/16 16:23:09 by roybakker     #+#    #+#                 */
/*   Updated: 2021/03/25 11:28:55 by gbouwen       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server/webserver.hpp"

int     main(int argc, char**argv) {
    webserver   webserver;

    if (argc != 2)
	{
		std::cout << "Start the webserver like this: $>./webserv config_file" << std::endl;
        return -1;
	}
    try {
        webserver.load_configuration(argv[1]);
		webserver.establish_connection();
		webserver.run();
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}
