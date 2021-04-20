/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   webserver.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/30 16:28:34 by roybakker     #+#    #+#                 */
/*   Updated: 2021/04/15 11:39:17 by gbouwen       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_WEBSERVER_HPP
#define WEBSERV_WEBSERVER_HPP

//general includes
#include <fcntl.h>

//custom includes
#include "server.hpp"

class webserver {
public:
    enum        webserver_values{ valid_ = 7, invalid_ = 8 };

private:
    std::vector<server>     _servers;
    fd_set			        _read_fds, _write_fds;
    fd_set			        _buffer_read_fds, _buffer_write_fds;
    int				        _highest_fd;

public:
    webserver();
    ~webserver();

    void    	load_configuration(char* config_file);
    void    	establish_connection();
    void    	run();

    //helper functions
    void    	initialize_fd_sets();
    void    	initialize_highest_fd();
    int			highest_fd(int fd_one, int fd_two);
    void    	add_sockets_to_read_fds();

    void    	accept_request();
    void   		handle_request();
    void    	read_requested_file();
    void    	create_response();

    int    		check_server_block(std::vector<std::string> server_block);

    //Debug-tool
    void        print_struct();
};

#endif //WEBSERV_WEBSERVER_HPP
