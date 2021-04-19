/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handler.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/07 13:10:33 by roybakker     #+#    #+#                 */
/*   Updated: 2021/04/07 13:10:33 by roybakker     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "handler.hpp"

//Debug tool
void handler::print_request() {
    std::cout << "------------- REQUEST -------------\n";

    std::cout << "Entity headers\n";
    std::cout << "  Content length = " << get_content_length() << std::endl;
    std::cout << "  Content type = " << get_content_type() << std::endl;
    std::cout << "  Content language = " << get_content_language() << std::endl;
    std::cout << "  Content location" << get_content_location() << std::endl;
    std::cout << "  Allow = " << get_allow() << std::endl;

    std::cout << "Request headers\n";
    std::cout << "  Method = " << get_method() << std::endl;
    std::cout << "  Location = " << get_location() << std::endl;
    std::cout << "  Protocol = " << get_protocol() << std::endl;
    std::cout << "  Host = " << get_host() << std::endl;
    std::cout << "  User agent = " << get_user_agent() << std::endl;
    std::cout << "  Accept language = " << get_accept_language() << std::endl;
    std::cout << "  Authorization = " << get_authorization() << std::endl;
    std::cout << "  Referer = " << get_referer() << std::endl;
    std::cout << "  Body = " << get_body() << std::endl;

    std::cout << "------------- END REQUEST -------------\n\n";
}
//------------------------------------------------------------


handler::handler() : _content_length(0), _content_type(), _content_language(), _content_location(), _allow(),
                     _method(), _location(), _protocol(), _host(), _user_agent(), _accept_language(), _authorization(), _referer(), _body(), _requested_file() {}
handler::~handler(){}

//Parse request functions
void        handler::parse_request(handler::location_vector location, int fd, handler::map request_buffer) {
    map_iterator request                        = request_buffer.find(fd);
    vector request_elements                     = str_to_vector(request->second);
    parse parse_array[12]                       = { &handler::parse_host,
                                                    &handler::parse_user_agent,
                                                    &handler::parse_language,
                                                    &handler::parse_authorization,
                                                    &handler::parse_referer,
                                                    &handler::parse_body,
                                                    &handler::parse_content_length,
                                                    &handler::parse_content_type,
                                                    &handler::parse_content_language,
                                                    &handler::parse_content_location,
                                                    &handler::parse_allow,
                                                    &handler::invalid_argument};

    clear_atributes();
    parse_first_line(*request_elements.begin());
    for (vector_iterator it = request_elements.begin(); it != request_elements.end(); it++) {
        int config_id = identify_request_value(*it);
        parse function = parse_array[config_id];
        (this->*function)(*it);
    }
    configure_location(location);
    std::cout << "-----------\n" << "request buffer = \n" << request->second << "-----------\n" << std::endl; //REMOVE
    print_request(); //REMOVE
}

int         handler::identify_request_value(const std::string &str) {
    if (str.find("Host") != std::string::npos)
        return host_;
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

void        handler::parse_first_line(const std::string &str) {
    size_t start = 0;
    size_t end = str.find_first_of(' ', start);
    _method = str.substr(start, end - start);

    start = end + 1;
    end = str.find_first_of(' ', start);
    _location = str.substr(start, end - start);
    _protocol = str.substr(end + 1);
}

void        handler::parse_host(const std::string &str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _host = str.substr(pos + 1);
}

void        handler::parse_user_agent(const std::string &str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _user_agent = str.substr(pos + 1);
}

void        handler::parse_language(const std::string &str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _accept_language = str.substr(pos + 1);
}

void        handler::parse_authorization(const std::string &str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _authorization = str.substr(pos + 1);
}

void        handler::parse_referer(const std::string &str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _referer = str.substr(pos + 1);
}

void        handler::parse_body(const std::string &str) {
    _body = str;
}

void        handler::parse_content_length(const std::string &str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    std::string tmp = str.substr(pos + 1);
    _content_length = ft_atoi(tmp.c_str());
}

void        handler::parse_content_type(const std::string &str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _content_type = str.substr(pos + 1);
}

void        handler::parse_content_language(const std::string &str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _content_language = str.substr(pos + 1);
}

void        handler::parse_content_location(const std::string &str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _content_location = str.substr(pos + 1);
}

void        handler::parse_allow(const std::string &str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _allow = str.substr(pos + 1);
}

void        handler::invalid_argument(const std::string &str) {
    if (str == "0")
        return;
}


//Create response functions

// Need to check later how to send correct response code

std::string	handler::generate_response_code(void)
{
	std::string	result = "HTTP/1.1 200 OK\n";

	return (result);
}

std::string	handler::generate_content_length(void)
{
	std::string	result = "Content-Length: ";

	result.append(ft_itoa(this->get_requested_file().size()));
	result.append("\n");
	return (result);
}

// Need to make dynamic

std::string	handler::generate_content_type(void)
{
	std::string	location = get_location();
	std::cout << "location: " << location << std::endl;
	std::string	result = "Content-Type: text/html; charset=UTF-8\n";

	return (result);
}

//std::string	handler::generate_last_modified(void)
//{
	//int			fd;
	//struct stat	statbuf;
	//struct tm	*info;
	//char		timestamp[36];
	//char		*result;

	//fd = open(location, O_RDONLY);
	//if (fd == -1)
		//// error
	//if (fstat(fd, &statbuf) == -1)
		//// error
	//info = localtime(&statbuf.st_mtime);
	//strftime(timestamp, 36, "%a, %d %m %Y %H:%M:%S GMT", timestamp);
	//result.append(timestamp);
	//return (result);
//}

handler::vector	handler::create_response_headers(void)
{
	vector		headers;
	std::string	temp;

	temp = generate_response_code();
	headers.push_back(temp);
	temp = generate_content_length();
	headers.push_back(temp);
	temp = generate_content_type();
	headers.push_back(temp);
	temp = "\n";
	headers.push_back(temp);
	return (headers);
}

void    handler::create_response_file(int io_fd, std::vector<std::string> headers)
{
	for (vector_iterator it = headers.begin(); it != headers.end(); it++)
	{
		std::string	header = *it;
		std::cout << "HEADERRR: " << header << std::endl;
		write(io_fd, header.c_str(), header.size());
	}
	std::cout << "FILEEE: " << this->get_requested_file() << std::endl;
    write(io_fd, this->get_requested_file().c_str(), this->get_requested_file().size());
}

//Helper functions
std::string    handler::read_browser_request(int fd) {
    std::string tmp;
    char        buff[3000];
    int         ret;

    while ((ret = read(fd, buff, 3000)) > 0) {
        tmp.append(buff, ret);
        if (ret < 3000)
            break;
    }
    if (ret == -1)
        return NULL; //need some error checking method
    return tmp;
}

void        handler::read_requested_file(int fd) {
    char    buff[3000];
    int     ret = 1;

    while (ret > 0) {
        ret = read(fd, buff, 3000);
        _requested_file.append(buff, ret);
        if (ret < 3000)
            break;
    }
    if (ret == -1) {
        return ; //need some error checking method
    }
}

int         handler::open_requested_file(std::string location) {
    char	*path;
    int		fd;

    path = &location[0];
    fd = open(path, O_RDONLY);
    if (fd == -1)
        return -1; //need some error checking method
    fcntl(fd, F_SETFL, O_NONBLOCK);
    return (fd);
}

handler::vector    handler::str_to_vector(std::string request) {
    std::vector<std::string> request_elements;
    size_t      pos;
    std::string value;

    while ((int)(pos = request.find_first_of('\n')) != -1) {
        value = request.substr(0, pos);
        request_elements.push_back(value);
        request = request. substr(pos + 1);
    }
    request_elements.push_back(request);
    return request_elements;
}

void        handler::configure_location(handler::location_vector location) {
    std::string request_location;
    if (_location[_location.length() - 1] == '/')
        request_location = _location;
    else {
        size_t pos = _location.find_last_of('/');
        request_location = _location.substr(0, pos+1); //only add 1 at index 0 or every time check later!
    }

    for (location_iterator loc = location.begin(); loc != location.end(); loc++) {
        if (loc->get_location() == request_location) {
            _location = loc->get_root().append(_location);
            if (get_file_extension(loc->get_ext()).compare("ext not found") == 0)
			{
				std::cout << "LOACITON: " << _location << std::endl;
				_location = _location.append(loc->get_index());
			}
			else
			{

				std::cout << "LOACITON: " << _location << std::endl;
			}
            return;
        }
    }
}

void		handler::clear_requested_file() {_requested_file.clear();}

void        handler::clear_atributes() {
    _content_length = 0;
    _method.clear();
    _location.clear();
    _protocol.clear();
    _host.clear();
    _user_agent.clear();
    _accept_language.clear();
    _authorization.clear();
    _referer.clear();
    _body.clear();
    _content_type.clear();
    _content_language.clear();
    _content_location.clear();
    _allow.clear();
}

std::string	handler::get_file_extension(handler::vector extensions)
{
	std::string	tmp = "ext not found";

	for (vector_iterator ext = extensions.begin(); ext != extensions.end(); ext++) {
		tmp = *ext;
		if (_location.find(tmp) != std::string::npos)
			return (tmp);
	}
	return (tmp);
}

//Getter
int             handler::get_content_length() { return _content_length;}
std::string     handler::get_content_type() { return _content_type;}
std::string     handler::get_content_language() { return _content_language;}
std::string     handler::get_content_location() { return _content_location;}
std::string     handler::get_allow() { return _allow;}
std::string	    handler::get_requested_file() { return (_requested_file);}
std::string     handler::get_method() { return _method;}
std::string     handler::get_location() { return _location;}
std::string     handler::get_protocol() { return _protocol;}
std::string     handler::get_host() { return _host;}
std::string     handler::get_user_agent() { return _user_agent;}
std::string     handler::get_accept_language() { return _accept_language;}
std::string     handler::get_authorization() { return _authorization;}
std::string     handler::get_referer() { return _referer;}
std::string     handler::get_body() { return _body;}
