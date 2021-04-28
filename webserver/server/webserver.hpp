/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   webserver.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/30 16:28:34 by roybakker     #+#    #+#                 */
/*   Updated: 2021/04/26 18:04:41 by gbouwen       ########   odam.nl         */
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
    enum        webserver_values{ error_page_ = -2, unused_ = -1, ready_for_use_ = -1, valid_ = 7, invalid_ = 8, error_code_ = 399 };

private:
    std::vector<server>     _servers;
    fd_set			        _readFDS, _writeFDS;
    fd_set			        _buffer_readFDS, _buffer_writeFDS;
    int				        _maxFD;

public:
    webserver();
    ~webserver();

    void    	load_configuration(char* config_file);
    void    	establish_connection();
    void    	run();

    //Helper functions
	void    	synchronize_FD_sets();
    void    	initialize_FD_sets();
    void    	initialize_highest_fd();
    void		set_maxFD(int fd);
	int			get_maxFD();
    int    		check_server_block(vector server_block);
	void		initialize_temp_files();

    //Debug-tool
    void        print_struct();
};

#endif //WEBSERV_WEBSERVER_HPP
