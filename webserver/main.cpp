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

    if (argc != 2) //only config file is allowed
        return -1;
    try {
        webserver.load_configuration(argv[1]);
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}
