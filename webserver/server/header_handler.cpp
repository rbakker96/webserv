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

header_handler::header_handler() : _content_length(0), _content_type("text/"), _content_language("en"), _content_location(), _allow(),
								   _method(), _file_location(), _protocol(), _requested_host(), _user_agent(), _accept_language(), _authorization(), _referer(), _body(), _requested_file() {}
header_handler::~header_handler(){}

//Parse request functions
void        header_handler::parse_request(header_handler::location_vector location, int fd, header_handler::map request_buffer) {
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

	clear_attributes();
    parse_first_line(*request_elements.begin());
    for (vector_iterator it = request_elements.begin(); it != request_elements.end(); it++) {
        int request_value = identify_request_value(*it);
        parse function = parse_array[request_value];
        (this->*function)(*it);
    }
    configure_location(location);
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

void        header_handler::parse_requested_host(const std::string &str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

	_requested_host = str.substr(pos + 1);
}

void        header_handler::parse_user_agent(const std::string &str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _user_agent = str.substr(pos + 1);
}

void        header_handler::parse_language(const std::string &str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _accept_language = str.substr(pos + 1);
}

void        header_handler::parse_authorization(const std::string &str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _authorization = str.substr(pos + 1);
}

void        header_handler::parse_referer(const std::string &str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _referer = str.substr(pos + 1);
}

void        header_handler::parse_body(const std::string &str) {
    _body = str;
}

void        header_handler::parse_content_length(const std::string &str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    std::string tmp = str.substr(pos + 1);
    _content_length = ft_atoi(tmp.c_str());
}

void        header_handler::parse_content_type(const std::string &str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _content_type = str.substr(pos + 1);
}

void        header_handler::parse_content_language(const std::string &str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _content_language = str.substr(pos + 1);
}

void        header_handler::parse_content_location(const std::string &str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _content_location = str.substr(pos + 1);
}

void        header_handler::parse_allow(const std::string &str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _allow = str.substr(pos + 1);
}

void        header_handler::invalid_argument(const std::string &str) {
    if (str == "0")
        return;
}

// Create response functions
// Need to check later how to send correct response code

void header_handler::send_response(int io_fd)
{
	std::string response;
	
	generate_status_line(response);
	generate_content_length(response);
	generate_content_type(response);
	response.append("\r\n");

	write(io_fd, response.c_str(), response.size());
	write(io_fd, this->get_requested_file().c_str(), this->get_requested_file().size());

}

void	header_handler::generate_status_line(std::string &response)
{
	std::string	status_line = "HTTP/1.1 200 OK";
	status_line.append("\r\n");
	response.append(status_line);
}

void	header_handler::generate_content_length(std::string &response)
{
	std::string	content_length = "Content-Length: ";

	content_length.append(ft_itoa(this->get_requested_file().size()));
	content_length.append("\r\n");
	response.append(content_length);
}

void	header_handler::generate_content_type(std::string &response) {
	std::string content_type = _content_type.append("; charset=UTF-8\n");
	content_type.append("\r\n");
	response.append(content_type);
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

//Helper functions
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
        return NULL; //need some error checking method
    return tmp;
}

void        header_handler::read_requested_file(int fd) {
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

int         header_handler::open_requested_file(std::string location) {
    char	*path;
    int		fd;

    path = &location[0];
    fd = open(path, O_RDONLY);
    if (fd == -1)
        return -1; //need some error checking method
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

void        header_handler::configure_location(header_handler::location_vector location_blocks) {
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
        request_location = _file_location.substr(0, pos+1); //only add 1 at index 0 or every time check later!
    }

    for (location_iterator loc = location_blocks.begin(); loc != location_blocks.end(); loc++) {
//		std::string locc = loc->get_location_context(); // not used
        if (loc->get_location_context() == request_location) {
			_file_location = loc->get_root().append(_file_location);
			std::vector<std::string> accepted_exts = loc->get_ext();
			for (vector_iterator ext = accepted_exts.begin(); ext != accepted_exts.end(); ext++) {
				if (_file_location.find(*ext) != std::string::npos) {
				    if (*ext == "png")
				        _content_type = "image/";
				    _content_type = _content_type.append(*ext);
                    return;
				}
			}
			_content_type = _content_type.append("html");
			_file_location = _file_location.append(loc->get_index());
            return;
        }
    }
}

void		header_handler::clear_requested_file() {_requested_file.clear();}

void        header_handler::clear_attributes() {
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
    _content_language.clear();
    _content_location.clear();
    _allow.clear();
}

//Getter
int             header_handler::get_content_length() { return _content_length;}
std::string     header_handler::get_content_type() { return _content_type;}
std::string     header_handler::get_content_language() { return _content_language;}
std::string     header_handler::get_content_location() { return _content_location;}
std::string     header_handler::get_allow() { return _allow;}
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