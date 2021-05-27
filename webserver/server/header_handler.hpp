/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handler.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/07 13:23:53 by roybakker     #+#    #+#                 */
/*   Updated: 2021/05/27 11:23:17 by gbouwen       ########   odam.nl         */
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
#include "request_buf.hpp"
#include "Base64.hpp"

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
	typedef		std::map<int, request_buf>				map;
	typedef		std::map<int, request_buf>::iterator	map_iterator;
    typedef		std::list<std::string>				    list;
    typedef		std::list<std::string>::iterator	    list_iterator;
	typedef		std::vector<location_context>			location_vector;
	typedef     std::vector<location_context>::iterator location_iterator;
	typedef		std::pair<int, std::string>				pair;

	typedef		void (header_handler::*parse)(const std::string &str);

	enum		location_values{requested_host_ = 0, user_agent_ = 1, accept_charset_ = 2,
								accept_language_ = 3, authorization_ = 4,
								referer_ = 5, content_length_ = 6, content_type_ = 7,
								content_language_ = 8, content_location_ = 9,
								special_x_header_ = 10, unknown_ = 11, error_code_ = 400,
								folder_ = -1, unused_ = -1};
	enum		status_values{	okay_ = 200, created_ = 201, no_content_ = 204,
								bad_request_ = 400, unauthorized_ = 401, forbidden_ = 403,
								not_found_ = 404, method_not_allowed_ = 405,
								payload_too_large_ = 413 };

protected:
//	int				_index;

	//status
	int				            _status;
    std::map<int, std::string>	_status_phrases;

	//Headers
    int             _max_file_size;
	int				_content_length;
	std::string		_content_type;
	std::string		_content_language;
	std::string		_content_location;
	vector			_allow;
	std::string		_method;
	std::string		_file_location;
	std::string 	_uri_location;
	std::string		_location_block_root;
	std::string		_protocol;
	std::string		_requested_host;
	std::string		_user_agent;
	std::string     _accept_charset;
	std::string		_accept_language;
	std::string		_authorization;
	std::string		_referer;
	std::string		_body;
	std::vector<std::string> _special_x_header;
	int             _location_index;
	std::string     _auth_basic;
	std::string     _auth_type;

	//Response
	std::string		_response_file;
	std::string     _additional_cgi_headers;

public:
	header_handler();
	~header_handler();

    //PARSE functions
	void			parse_request(request_buf request_buffer);
	int				identify_request_value(const std::string &str);
	void			parse_first_line(const std::string &str);
	void			parse_requested_host(const std::string &str);
	void			parse_user_agent(const std::string &str);
	void			parse_accept_charset(const std::string &str);
	void            parse_accept_language(const std::string &str);
	void            parse_authorization(const std::string &str);
	void            parse_referer(const std::string &str);
	void            parse_body(request_buf request);
	void            parse_content_length(const std::string &str);
	void            parse_content_type(const std::string &str);
	void            parse_content_language(const std::string &str);
	void            parse_content_location(const std::string &str);
	void        	parse_special_x_header(const std::string &str);
	void            invalid_argument(const std::string &str);

    //HANDLE functions
    int handle_request(std::string cgi_file_types, location_vector location_blocks, std::string error_page, int index,
                       bool *authorization_status);
    int             put_request();
    int             post_request(int max_file_size);
    void            write_body_to_file(int file_fd);
	int 			create_cgi_fd(std::string type, int index);
    void            verify_file_location(location_vector location_blocks, std::string error_page);
	std::string		match_location_block(header_handler::location_vector location_blocks, std::string file_location);
	std::string		generate_error_page_location(std::string error_page);
	std::string		get_referer_part();
	std::string		location_of_uploaded_file(location_context location_block, std::string location, std::string uri_location, std::string extension);
    void            verify_method(std::string cgi_file_types);
    std::string     verify_content_type();
	void verify_authorization(location_context location_block, bool *authorization_status);


	//CGI functions
	void execute_cgi(int inputFD, int outputFD, std::string server_name, int server_port, bool auth_status,
	                 std::string auth_info);
	char 			**create_cgi_args();
	char **create_cgi_envp(const std::string &server_name, int server_port, bool auth_status, std::string auth_info);

    //RESPONSE functions
    void            read_requested_file(int fd);
    void            read_cgi_header_file(int fd, int body_size);
	void            send_response(int activeFD, int fileFD, std::string server_name);

    //RESET functions
    void            reset_handler();

    //GET functions
    int             get_max_file_size();
	int             get_status();
    int             get_content_length();
    std::string     get_content_type();
    std::string     get_content_language();
    std::string     get_content_location();
	vector			get_allow();
	std::string	    get_response_file();
    std::string     get_method();
    std::string     get_file_location();
    std::string     get_uri_location();
	std::string		get_location_block_root();
    std::string     get_protocol();
    std::string     get_requested_host();
    std::string     get_user_agent();
    std::string     get_accept_language();
    std::string     get_accept_charset();
    std::string     get_authorization();
    std::string     get_referer();
    std::string     get_body();

    //DEBUG functions
    void            print_request();
    void            print_response(std::string response);
};

#endif //WEBSERV_HANDLER_HPP
