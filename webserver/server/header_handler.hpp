/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handler.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/07 13:23:53 by roybakker     #+#    #+#                 */
/*   Updated: 2021/05/11 14:24:48 by gbouwen       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HANDLER_HPP
#define WEBSERV_HANDLER_HPP

//General includes
#include <map>
#include <string>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

//Custom includes
#include "location_context.hpp"
#include "../helper/helper.hpp"

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

class header_handler {
public:
	typedef		std::vector<std::string>				vector;
	typedef		std::vector<std::string>::iterator		vector_iterator;
	typedef		std::map<int, std::string>				map;
	typedef		std::map<int, std::string>::iterator	map_iterator;
	typedef		std::vector<location_context>			location_vector;
	typedef     std::vector<location_context>::iterator location_iterator;
	typedef		std::pair<int, std::string>				pair;

	typedef		void (header_handler::*parse)(const std::string &str);

	enum		location_values{requested_host_ = 0, user_agent_ = 1, language_ = 2, authorization_ = 3,
								referer_ = 4, body_ = 5, content_length_ = 6, content_type_ = 7,
								content_language_ = 8, content_location_ = 9, allow_ = 10, unknown_ = 11,
								error_code_ = 400, folder_ = -1, unused_ = -1};
	enum		status_values{	okay_ = 200, created_ = 201, no_content_ = 204,
								bad_request_ = 400, unauthorized_ = 401, forbidden_ = 403,
								not_found_ = 404, method_not_allowed_ = 405,
								payload_too_large_ = 413	};

protected:
	int				_index;

	//status
	int				_status;
	map				_status_phrases;

	//Entity headers                        //why not response headers
	int				_content_length;
	std::string		_content_type;
	std::string		_content_language;
	std::string		_content_location;
	std::string		_allow;                 //difference with vector?
	vector			_allowed_methods_config;

	//Request headers
	std::string		_method;
	std::string		_file_location;
	std::string		_protocol;
	std::string		_requested_host;
	std::string		_user_agent;
	std::string		_accept_language;
	std::string		_authorization;
	std::string		_referer;
	std::string		_body;

	std::string		_requested_file;

public:
	header_handler();
	~header_handler();

    //Parse request functions
    void            parse_request(int fd, map request_buffer);
    void            parse_first_line(const std::string &str);
    void            parse_requested_host(const std::string &str);
    void            parse_user_agent(const std::string &str);
    void            parse_language(const std::string &str);
    void            parse_authorization(const std::string &str);
    void            parse_referer(const std::string &str);
    void            parse_body(const std::string &str);
    int             identify_request_value(const std::string &str);

    void            parse_content_length(const std::string &str);
    void            parse_content_type(const std::string &str);
    void            parse_content_language(const std::string &str);
    void            parse_content_location(const std::string &str);
    void            parse_allow(const std::string &str);
    void            invalid_argument(const std::string &str);

    //Handle request functions
    int             handle_request(location_vector location_blocks, std::string error_page, int max_file_size);
    int             put_request(int max_file_size);
    void            write_put_file(int file_fd);
	int             cgi_request();
    void            verify_file_location(location_vector location_blocks, std::string error_page);
	int				match_location_block(header_handler::location_vector location_blocks, std::string file_location);
	std::string		generate_error_page_location(std::string error_page);
	std::string		get_referer_part();
    void 		    verify_method();

	//CGI functions
	void			execute_php(int fileFD, std::string server_name, int server_port);
	char 			**create_cgi_args();
	char 			**create_cgi_envp(const std::string &server_name, int server_port);

    //Send response functions
	std::string     send_response(int activeFD, int fileFD, std::string server_name);
	void			generate_status_line(std::string &response);
	void			generate_content_length(std::string &response);
	void			generate_content_type(std::string &response);
	void			generate_last_modified(std::string &response, int file_fd);
	void			generate_date(std::string &response);
	void			generate_server_name(std::string &response, std::string server_name);
	void			generate_allowed_methods_config(std::string &response);
	void 			add_connection_close(std::string &response);

    //Helper functions
    std::string     read_browser_request(int fd);
    std::string     verify_content_type();
    vector          str_to_vector(std::string request);
    void            read_requested_file(int fd);
    void		    clear_requested_file();
    void            reset_handler_atributes();
    void            reset_status();

    //Getter
	int				get_index();
	int             get_status();
    int             get_content_length();
    std::string     get_content_type();
    std::string     get_content_language();
    std::string     get_content_location();
    std::string     get_allow();
	vector			get_allowed_methods_config();
	std::string	    get_requested_file();
    std::string     get_method();
    std::string     get_file_location();
    std::string     get_protocol();
    std::string     get_requested_host();
    std::string     get_user_agent();
    std::string     get_accept_language();
    std::string     get_authorization();
    std::string     get_referer();
    std::string     get_body();

	//Setter
	void			set_index(int index);

    //Debug tool
    void            print_request(); //DELETE LATER
};

#endif //WEBSERV_HANDLER_HPP
