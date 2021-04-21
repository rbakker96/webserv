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
	typedef     std::vector<std::string>      			vector;
	typedef     std::vector<std::string>::iterator      vector_iterator;
    enum        webserver_values{ unused_ = -1, ready_for_use_ = -1, valid_ = 7, invalid_ = 8 };

private:
    std::vector<server>     _servers;
    fd_set			        _read_fds, _write_fds;
    fd_set			        _buffer_read_fds, _buffer_write_fds;
    int				        _maxFD;

public:
    webserver();
    ~webserver();

    void    	load_configuration(char* config_file);
    void    	establish_connection();
    void    	run();

    //Helper functions
	void    	sychronize_FD_sets();
    void    	initialize_FD_sets();
    void    	initialize_highest_fd();
    void		set_maxFD(int fd);
	int			get_maxFD();
    int    		check_server_block(vector server_block);

    //Debug-tool
    void        print_struct();
};

#endif //WEBSERV_WEBSERVER_HPP
