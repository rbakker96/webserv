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
#include "response.hpp"
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
								content_language_ = 8, content_location_ = 9, unknown_ = 10,
								error_code_ = 400, folder_ = -1, unused_ = -1};
	enum		status_values{	okay_ = 200, created_ = 201, no_content_ = 204,
								bad_request_ = 400, unauthorized_ = 401, forbidden_ = 403,
								not_found_ = 404, method_not_allowed_ = 405,
								payload_too_large_ = 413 };

protected:
	int				_index;

	//status
	int				_status;
	map				_status_phrases;

	//Headers
	int				_content_length;
	std::string		_content_type;
	std::string		_content_language;
	std::string		_content_location;
	vector			_allow;
	std::string		_method;
	std::string		_file_location;
	std::string		_protocol;
	std::string		_requested_host;
	std::string		_user_agent;
	std::string		_accept_language;
	std::string		_authorization;
	std::string		_referer;
	std::string		_body;

	//Response
	std::string		_response_file;

public:
	header_handler();
	~header_handler();

    //PARSE functions
    void            parse_request(int fd, map request_buffer);
    int             identify_request_value(const std::string &str);
    void            parse_first_line(const std::string &str);
    void            parse_requested_host(const std::string &str);
    void            parse_user_agent(const std::string &str);
    void            parse_language(const std::string &str);
    void            parse_authorization(const std::string &str);
    void            parse_referer(const std::string &str);
    void            parse_body(const std::string &str);
    void            parse_content_length(const std::string &str);
    void            parse_content_type(const std::string &str);
    void            parse_content_language(const std::string &str);
    void            parse_content_location(const std::string &str);
    void            invalid_argument(const std::string &str);

    //HANDLE functions
    int             handle_request(std::string cgi_file_types, location_vector location_blocks, std::string error_page, int max_file_size);
    int             put_request(int max_file_size);
    void            write_put_file(int file_fd);
	int             cgi_request();
    void            verify_file_location(location_vector location_blocks, std::string error_page);
	int				match_location_block(header_handler::location_vector location_blocks, std::string file_location);
	std::string		generate_error_page_location(std::string error_page);
	std::string		get_referer_part();
    void 		    verify_method();
    std::string     verify_content_type();

	//CGI functions
	void			execute_cgi(int fileFD, std::string server_name, int server_port);
	char 			**create_cgi_args();
	char 			**create_cgi_envp(const std::string &server_name, int server_port);

    //RESPONSE functions
    void            read_requested_file(int fd);
	void            send_response(int activeFD, int fileFD, std::string server_name);

    //RESET functions
    void            reset_handler();

    //GET functions
	int				get_index();
	int             get_status();
    int             get_content_length();
    std::string     get_content_type();
    std::string     get_content_language();
    std::string     get_content_location();
	vector			get_allow();
	std::string	    get_response_file();
    std::string     get_method();
    std::string     get_file_location();
    std::string     get_protocol();
    std::string     get_requested_host();
    std::string     get_user_agent();
    std::string     get_accept_language();
    std::string     get_authorization();
    std::string     get_referer();
    std::string     get_body();

	//SET functions
	void			set_index(int index);

    //DEBUG functions
    void            print_request(std::string request);
    void            print_response(std::string response);
};

#endif //WEBSERV_HANDLER_HPP
