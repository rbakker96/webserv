/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   request_handler.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/07 10:45:44 by roybakker     #+#    #+#                 */
/*   Updated: 2021/04/07 10:45:44 by roybakker     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

//before select
//GET / HTTP/1.1
//Host: localhost:8080
//Upgrade-Insecure-Requests: 1
//Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
//User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_6) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/14.0.3 Safari/605.1.15
//Accept-Language: en-us
//Accept-Encoding: gzip, deflate
//Connection: keep-alive
//
//GET /style.css HTTP/1.1
//Host: localhost:8080
//Connection: keep-alive
//Accept: text/css,*/*;q=0.1
//User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_6) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/14.0.3 Safari/605.1.15
//Accept-Language: en-us
//Referer: http://localhost:8080/
//Accept-Encoding: gzip, deflate

#include "request_handler.hpp"

void request_handler::print_request() {
    std::cout << "------------- REQUEST -------------\n\n";

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


request_handler::request_handler() : _method(), _location(), _protocol(), _host(), _user_agent(), _accept_language(), _authorization(), _referer(), _body() {}
request_handler::~request_handler(){}

//Parse functions
void    request_handler::parse_request(std::vector<location_context> location, int fd, std::map<int, std::string> request_buffer) {
    map_iterator request                        = request_buffer.find(fd);
    std::vector<std::string> request_elements   = str_to_vector(request->second);
    parse parse_array[12]                       = { &request_handler::parse_host,
                                                    &request_handler::parse_user_agent,
                                                    &request_handler::parse_language,
                                                    &request_handler::parse_authorization,
                                                    &request_handler::parse_referer,
                                                    &request_handler::parse_body,
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


int     request_handler::identify_request_value(const std::string &str) {
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

void    request_handler::parse_first_line(const std::string &str) {
    size_t start = 0;
    size_t end = str.find_first_of(' ', start);
    _method = str.substr(start, end - start);

    start = end + 1;
    end = str.find_first_of(' ', start);
    _location = str.substr(start, end - start);
    _protocol = str.substr(end + 1);
}

void    request_handler::parse_host(const std::string &str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _host = str.substr(pos + 1);
}

void    request_handler::parse_user_agent(const std::string &str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _user_agent = str.substr(pos + 1);
}

void    request_handler::parse_language(const std::string &str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _accept_language = str.substr(pos + 1);
}

void    request_handler::parse_authorization(const std::string &str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _authorization = str.substr(pos + 1);
}

void    request_handler::parse_referer(const std::string &str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _referer = str.substr(pos + 1);
}

void    request_handler::parse_body(const std::string &str) {
    _body = str;
}


//Helper functions
std::vector<std::string>    request_handler::str_to_vector(std::string request) {
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

void    request_handler::configure_location(std::vector<location_context> location) {
    // /style.css -> right location not found

    for(loc_iterator it = location.begin(); it != location.end(); it++) {
//        std::vector<std::string> extensions = it->get_ext();
//        for (vector_iterator it1 = extensions.begin(); it1 != extensions.end(); it1++) {
//            std::string tmp = *it1;
//            if (_location.find(tmp) != std::string::npos)
//                return;
//        }

        if (it->get_location() == _location) {
            _location = it->get_root().append(_location);
            std::cout << "temp location = " << _location << std::endl;
            std::vector<std::string> extensions = it->get_ext();
            for (vector_iterator it2 = extensions.begin(); it2 != extensions.end(); it2++) {
                std::string tmp = *it2;
                if (_location.find(tmp) != std::string::npos)
                    return;
            }
            if (_location.back() != '/')
                _location = _location.append("/");
            _location = _location.append(it->get_index());
            return;
        }
    }
}

int     request_handler::open_requested_file(std::string location) {
	char	*path;
	int		fd;

	path = &location[0];
    fd = open(path, O_RDONLY);
    if (fd == -1)
        return -1; //need some error checking method
    fcntl(fd, F_SETFL, O_NONBLOCK);
    return (fd);
}

void    request_handler::clear_atributes() {
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
     _content_length = 0;
}

//Getter
std::string     request_handler::get_method() { return _method;}
std::string     request_handler::get_location() { return _location;}
std::string     request_handler::get_protocol() { return _protocol;}
std::string     request_handler::get_host() { return _host;}
std::string     request_handler::get_user_agent() { return _user_agent;}
std::string     request_handler::get_accept_language() { return _accept_language;}
std::string     request_handler::get_authorization() { return _authorization;}
std::string     request_handler::get_referer() { return _referer;}
std::string     request_handler::get_body() { return _body;}

