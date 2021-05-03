/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   header_handler.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: gbouwen <marvin@codam.nl>                    +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/03 12:34:40 by gbouwen       #+#    #+#                 */
/*   Updated: 2021/05/03 12:41:04 by gbouwen       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handler.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/07 13:10:33 by roybakker     #+#    #+#                 */
/*   Updated: 2021/05/03 12:34:39 by gbouwen       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "header_handler.hpp"

//Debug tool
void header_handler::print_request() {
    std::cout << "------------- REQUEST -------------\n";

    std::cout << "Entity headers\n";
    std::cout << "  Content length = " << get_content_length() << std::endl;
    std::cout << "  Content type = " << get_content_type() << std::endl;
    std::cout << "  Content language = " << get_content_language() << std::endl;
    std::cout << "  Content location" << get_content_location() << std::endl;
    std::cout << "  Allow = " << get_allow() << std::endl;

    std::cout << "Request headers\n";
    std::cout << "  Method = " << get_method() << std::endl;
    std::cout << "  Location = " << get_file_location() << std::endl;
    std::cout << "  Protocol = " << get_protocol() << std::endl;
    std::cout << "  Host = " << get_requested_host() << std::endl;
    std::cout << "  User agent = " << get_user_agent() << std::endl;
    std::cout << "  Accept language = " << get_accept_language() << std::endl;
    std::cout << "  Authorization = " << get_authorization() << std::endl;
    std::cout << "  Referer = " << get_referer() << std::endl;
    std::cout << "  Body = " << get_body() << std::endl;

    std::cout << "------------- END REQUEST -------------\n\n";
}
//------------------------------------------------------------


header_handler::header_handler() : _status(200), _content_length(0), _content_type("Content-Type: text/"), _content_language("en"), _content_location(), _allow(),
								   _method(), _file_location(), _protocol(), _requested_host(), _user_agent(), _accept_language(), _authorization(), _referer(), _body(), _requested_file() {
    _status_phrases.insert (pair(200, "OK"));
    _status_phrases.insert (pair(400, "Bad Request"));
    _status_phrases.insert (pair(401, "Unauthorized"));
    _status_phrases.insert (pair(403, "Forbidden"));
    _status_phrases.insert (pair(404, "Not Found"));
    _status_phrases.insert (pair(405, "Method Not Allowed"));
//    _status_phrases.insert ( pair(, ""));
//    _status_phrases.insert ( pair(, ""));
//    _status_phrases.insert ( pair(, ""));
}

header_handler::~header_handler(){
    _status_phrases.clear();
}

//------Parse request functions------
void        header_handler::parse_request(int fd, header_handler::map request_buffer) {
    map_iterator request                        = request_buffer.find(fd);
    vector request_elements                     = str_to_vector(request->second);
    parse parse_array[12]                       = { &header_handler::parse_requested_host,
                                                    &header_handler::parse_user_agent,
                                                    &header_handler::parse_language,
                                                    &header_handler::parse_authorization,
                                                    &header_handler::parse_referer,
                                                    &header_handler::parse_body,
                                                    &header_handler::parse_content_length,
                                                    &header_handler::parse_content_type,
                                                    &header_handler::parse_content_language,
                                                    &header_handler::parse_content_location,
                                                    &header_handler::parse_allow,
                                                    &header_handler::invalid_argument};

    reset_handler_atributes();
    parse_first_line(*request_elements.begin());
    for (vector_iterator it = request_elements.begin(); it != request_elements.end(); it++) {
        int request_value = identify_request_value(*it);
        parse function = parse_array[request_value];
        (this->*function)(*it);
    }
//    configure_location(location);
    std::cout << "-----------\n" << "request buffer = \n" << request->second << "-----------\n" << std::endl; //REMOVE
    print_request(); //REMOVE
}

int         header_handler::identify_request_value(const std::string &str) {
    if (str.find("Host") != std::string::npos)
        return requested_host_;
    else if (str.find("User-Agent") != std::string::npos)
        return user_agent_;
    else if (str.find("Accept-Language") != std::string::npos)
        return language_;
    else if (str.find("Authorization:") != std::string::npos)
        return authorization_;
    else if (str.find("Referer:") != std::string::npos)
        return referer_;
    else if ((int)str.length() == _content_length && _content_length != 0)
        return body_;
    else if (str.find("Content-Length:") != std::string::npos)
        return content_length_;
    else if (str.find("Content-Type:") != std::string::npos)
        return content_type_;
    else if (str.find("Content-Language:") != std::string::npos)
        return content_language_;
    else if (str.find("Content-Location:") != std::string::npos)
        return content_location_;
    else if (str.find("Allow:") != std::string::npos)
        return allow_;
    return unknown_;
}

void        header_handler::parse_first_line(const std::string &str) {
    size_t start = 0;
    size_t end = str.find_first_of(' ', start);
    _method = str.substr(start, end - start);

    start = end + 1;
    end = str.find_first_of(' ', start);
	_file_location = str.substr(start, end - start);
    _protocol = str.substr(end + 1);
}

void        header_handler::parse_requested_host(const std::string &str) {_requested_host = parse_string(str);}
void        header_handler::parse_user_agent(const std::string &str) {_user_agent = parse_string(str);}
void        header_handler::parse_language(const std::string &str) {_accept_language = parse_string(str);}
void        header_handler::parse_authorization(const std::string &str) {_authorization = parse_string(str);}
void        header_handler::parse_referer(const std::string &str) {_referer = parse_string(str);}
void        header_handler::parse_body(const std::string &str) {_body = str;}
void        header_handler::parse_content_length(const std::string &str) {_content_length = parse_number(str);}
void        header_handler::parse_content_type(const std::string &str) {_content_type = parse_string(str);}
void        header_handler::parse_content_language(const std::string &str) {_content_language = parse_string(str);}
void        header_handler::parse_content_location(const std::string &str) {_content_location = parse_string(str);}
void        header_handler::parse_allow(const std::string &str) {_allow = parse_string(str);}
void        header_handler::invalid_argument(const std::string &str) {parse_invalid(str);}


//------Handle request functions------
int        header_handler::handle_request() {
    if (_method == "GET" || _method == "HEAD")
        return get_request();
    else if (_method == "POST")
        return post_request();
//    else if (_method == "")
//        ;
    else if (_method == "PUT")
        return put_request();
    return 0;
}

int        header_handler::get_request() {
    //check for existing location block
        //not present redirect to error location
        //present check for extension to see if index is needed

    //determine content type

    //if php file and it exists
        //open temp file and return that to _fileFD

    //else Open requested file
        //open failed -> send back 500 (internal server errror)
        //open succes -> return fd to set as activeFD

    return (open_requested_file(_file_location));
}

int			header_handler::post_request()
{
	std::string	str_filename = "server_files/www/temp";
	char	*index_str = ft_itoa(_index);

	str_filename.append(index_str);
	free(index_str);
	const char *filename = str_filename.c_str();
	int	cgiFD = open(filename, O_CREAT | O_RDWR | O_TRUNC, S_IRWXU);
	if (cgiFD == -1)
		throw std::runtime_error("Open failed");
	fcntl(cgiFD, F_SETFL, O_NONBLOCK);
	return (cgiFD);
}

int         header_handler::put_request() {
    //not allowed method (405)
    //bigger then max filesize (413)
    //uploading older version then present (409) conflict
    //succesfully created (201)
    //succecfully modified (204)

    return 0;
}

//------CGI functions------
void		header_handler::execute_php(int fileFD)
{
	char	**args = new char *[3];

	args[0] = ft_strdup("/usr/bin/php");
	args[1] = ft_strdup(_file_location.c_str());
	args[2] = NULL;
	if (fork() == 0) // needs error checking
	{
		close(STDOUT_FILENO);
		dup2(fileFD, STDOUT_FILENO);
		execve(args[0], args, NULL);
	}
	else
		wait(NULL);
	free(args[0]);
	free(args[1]);
	delete [] args;
}

//------Send response functions------
void        header_handler::send_response(int activeFD, int fileFD, std::string server_name) {
	std::string response;

	generate_status_line(response);
	generate_content_length(response);
	generate_content_type(response);
	generate_last_modified(response, fileFD);
	generate_date(response);
	generate_server_name(response, server_name);
	generate_allowed_methods_config(response);
	response.append("\r\n");

	write(activeFD, response.c_str(), response.size());
	if (_method != "HEAD")
	    write(activeFD, this->get_requested_file().c_str(), this->get_requested_file().size());

	reset_status();
	clear_requested_file();
}

// Need to check later how to send correct response code

void	header_handler::generate_status_line(std::string &response) {
	std::string status_line = get_protocol();

	status_line.append(" ");
	status_line.append((ft_itoa(_status)));
    status_line.append(" ");
    status_line.append(_status_phrases[_status]);
	status_line.append("\r\n");

	response.append(status_line);
}

void	header_handler::generate_content_length(std::string &response){
	std::string	content_length = "Content-Length: ";

	content_length.append(ft_itoa(this->get_requested_file().size()));
	content_length.append("\r\n");
	response.append(content_length);
}

void	header_handler::generate_content_type(std::string &response) {
	std::string	content_type_header = "Content-Type: ";

	if (_content_type.compare("html") == 0 || _content_type.compare("css") == 0)
		content_type_header.append("text/");
	else if (_content_type.compare("png") == 0)
		content_type_header.append("image/");
	content_type_header.append(_content_type);
	content_type_header.append("\r\n");
	response.append(content_type_header);
}

void	header_handler::generate_last_modified(std::string &response, int fileFD)
{
	std::string	last_modified = "Last-Modified: ";
	struct stat	statbuf;
	struct tm	*info;
	char		timestamp[36];

	fstat(fileFD, &statbuf);
	info = localtime(&statbuf.st_mtime);
	strftime(timestamp, 36, "%a, %d %h %Y %H:%M:%S GMT", info);
	last_modified.append(timestamp);
	last_modified.append("\r\n");
	response.append(last_modified);
}

void	header_handler::generate_date(std::string &response)
{
	std::string	date = "Date: ";
	time_t		timer;
	struct tm	*info;
	char		timestamp[36];

	timer = time(NULL);
	info = localtime(&timer);
	strftime(timestamp, 36, "%a, %d %h %Y %H:%M:%S GMT", info);
	date.append(timestamp);
	date.append("\r\n");
	response.append(date);
}

void	header_handler::generate_server_name(std::string &response, std::string server_name)
{
	std::string server_header = "Server: ";

	server_header.append(server_name);
	server_header.append("\r\n");
	response.append(server_header);
}

// ONLY SEND WITH "405 Method Not Allowed" RESPONSE CODE!

void	header_handler::generate_allowed_methods_config(std::string &response)
{
	std::string allow_header = "Allow: ";

	for (header_handler::vector_iterator it = _allowed_methods_config.begin(); it != _allowed_methods_config.end(); it++)
	{
		std::string	method = *it;
		allow_header.append(method);
		if (it + 1 != _allowed_methods_config.end())
			allow_header.append(", ");
	}
	allow_header.append("\r\n");
	response.append(allow_header);
}

//------Helper functions------
std::string    header_handler::read_browser_request(int fd) {
    std::string tmp;
    char        buff[3000];
    int         ret;

    while ((ret = read(fd, buff, 3000)) > 0) {
        tmp.append(buff, ret);
        if (ret < 3000)
            break;
    }
    if (ret == -1)
        throw std::runtime_error("Read failed");
    return tmp;
}

void        header_handler::read_requested_file(int fd) {
    char    buff[3000];
    int     ret = 1;

	lseek(fd, 0, SEEK_SET);
    while (ret > 0) {
        ret = read(fd, buff, 3000);
        _requested_file.append(buff, ret);
        if (ret < 3000)
            break;
    }
    if (ret == -1)
        throw std::runtime_error("Read failed");
}

int         header_handler::open_requested_file(std::string file_location) {
    std::string error_page = "server_files/www/error_pages/.html";
    struct      stat stats;
    int		    fd;

    stat(file_location.c_str(), &stats);
    if (!(stats.st_mode & S_IRUSR))
        _status = forbidden_;
    if ((fd = open(&file_location[0], O_RDONLY)) == -1 )
        _status = not_found_;
    if (_status >= error_code_) {
        _file_location = error_page.insert(29, ft_itoa(_status));
//        _allow = "put"; something else later?
        fd = open(&_file_location[0], O_RDONLY);
        if (fd == -1)
            throw std::runtime_error("Open failed");
    }
    fcntl(fd, F_SETFL, O_NONBLOCK);
    return (fd);
}

header_handler::vector    header_handler::str_to_vector(std::string request) {
    std::vector<std::string> request_elements;
    size_t      pos;
    std::string value;

    while ((int)(pos = request.find_first_of('\r')) != -1) {
        value = request.substr(0, pos);
        request_elements.push_back(value);
        request = request. substr(pos + 2);
    }
    request_elements.push_back(request);
    return request_elements;
}

void        header_handler::configure_location(header_handler::location_vector location_blocks, std::string error_page) {
    std::string request_location = requested_location_block();

    for (location_iterator loc = location_blocks.begin(); loc != location_blocks.end(); loc++) {
        if (loc->get_location_context() == request_location) {
            _file_location = loc->get_root().append(_file_location);
            if (determine_content_type() == folder_)
                _file_location = _file_location.append(loc->get_index());
            return;
        }
    }
    size_t pos = _file_location.find("resources");
    if (pos != std::string::npos) {
        std::string tmp = _file_location.substr(pos - 1);
        _file_location = error_page.append(tmp);
    }
    else
        _file_location = error_page.append(_file_location);
    determine_content_type();
}

std::string header_handler::requested_location_block() {
    std::string request_location;
    size_t pos;

    if (!_referer.empty()) {
        pos = _referer.find(_requested_host);
        request_location = _referer.substr(pos + _requested_host.length());
    }
    else if (_file_location[_file_location.length() - 1] == '/')
        request_location = _file_location;
    else {
        pos = _file_location.find_last_of('/');
        request_location = _file_location.substr(0, pos+1);
    }
    return request_location;
}

int         header_handler::determine_content_type() {
    vector extensions;
    extensions.push_back("html");
    extensions.push_back("php");
    extensions.push_back("css");
    extensions.push_back("ico");
    extensions.push_back("png");

    for (vector_iterator it = extensions.begin(); it != extensions.end(); it++) {
        if (_file_location.find(*it) != std::string::npos) {
//            _content_type = *it;
            _content_type = _content_type.append(*it);
            return 1;
        }
    }
//    _content_type = "html";
    _content_type = _content_type.append("html");
    return (folder_);
}

void		header_handler::clear_requested_file() {_requested_file.clear();}

void        header_handler::reset_handler_atributes() {
    _content_length = 0;
    _method.clear();
    _file_location.clear();
    _protocol.clear();
    _requested_host.clear();
    _user_agent.clear();
    _accept_language.clear();
    _authorization.clear();
    _referer.clear();
    _body.clear();
	_content_type.clear();
    _content_language = "en";
    _content_location.clear();
    _allow.clear();
}

void            header_handler::reset_status() {_status = 200;}

//------Getter------
int				header_handler::get_index() { return _index; }
int             header_handler::get_content_length() { return _content_length;}
std::string     header_handler::get_content_type() { return _content_type;}
std::string     header_handler::get_content_language() { return _content_language;}
std::string     header_handler::get_content_location() { return _content_location;}
std::string     header_handler::get_allow() { return _allow;}
header_handler::vector	header_handler::get_allowed_methods_config() { return _allowed_methods_config; }
std::string	    header_handler::get_requested_file() { return (_requested_file);}
std::string     header_handler::get_method() { return _method;}
std::string     header_handler::get_file_location() { return _file_location;}
std::string     header_handler::get_protocol() { return _protocol;}
std::string     header_handler::get_requested_host() { return _requested_host;}
std::string     header_handler::get_user_agent() { return _user_agent;}
std::string     header_handler::get_accept_language() { return _accept_language;}
std::string     header_handler::get_authorization() { return _authorization;}
std::string     header_handler::get_referer() { return _referer;}
std::string     header_handler::get_body() { return _body;}

//------Setter------
void			header_handler::set_index(int index)
{
	_index = index;
}

