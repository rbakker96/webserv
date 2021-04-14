/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/30 13:54:06 by roybakker     #+#    #+#                 */
/*   Updated: 2021/03/30 13:54:06 by roybakker     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "../helper/helper.hpp"
#include <stdio.h>

server::server() {
    _tcp_socket = 0;
    _addr_len = 0;
}
server::~server(){}

void    server::create_new_server(std::vector <std::string> server_config) {
    location_context                location;
    configure configure_array[6] = { &server::configure_port,
                                     &server::configure_host,
                                     &server::configure_server_name,
                                     &server::configure_error_page,
                                     &server::configure_max_file_size,
                                     &server::invalid_element };

    clean_server_instance();
    for (std::vector<std::string>::iterator it = server_config.begin(); it != server_config.end(); it++) {
        int config_id = identify_server_value(*it);
        if (config_id == location_) {
            location.configure_location_context(it, (it + location_size(it, server_config.end())));
            _location.push_back(location);
        }
        else {
            configure function = configure_array[config_id];
            (this->*function)(*it);
        }
    }
}


//------Configure functions------
void    server::clean_server_instance(){
    _max_file_size = 0;
    _port = 0;
    _host.clear();
    _server_name.clear();
    _error_page.clear();
    _location.clear();
}

int     server::identify_server_value(std::string str) {
    if ((int)str.find("port") != -1)
        return port_;
    else if ((int)str.find("host") != -1)
        return host_;
    else if ((int)str.find("server_name") != -1)
        return server_name_;
    else if ((int)str.find("error_page") != -1)
        return error_page_;
    else if ((int)str.find("max_file_size") != -1)
        return max_file_size_;
    else if ((int)str.find("location") != -1)
        return location_;
    return unknown_;
}

void    server::configure_port(std::string str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);
    std::string temp = str.substr(pos + 1);

    _port = atoi(temp.c_str()); //NOT ALLOWED
}

void    server::configure_host(std::string str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _host = str.substr(pos + 1);
}

void    server::configure_server_name(std::string str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _server_name = str.substr(pos + 1);
}

void    server::configure_error_page(std::string str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);

    _error_page = str.substr(pos + 1);
}

void    server::configure_max_file_size(std::string str) {
    size_t start = str.find_first_not_of(' ');
    size_t pos = str.find_first_of(' ', start);
    std::string temp = str.substr(pos + 1);

    _max_file_size = atoi(temp.c_str()); //NOT ALLOWED
}

void    server::invalid_element(std::string str) {
    if (str == "0")
        return;
    return;
}


//------TCP-connection functions------
void	server::create_socket() {
    if ((this->_tcp_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw (std::runtime_error("Socket creation failed"));
    fcntl(this->_tcp_socket, F_SETFL, O_NONBLOCK);
}

void	server::bind_socket_address(int port) {
    memset((char *)&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = htonl(INADDR_ANY);
    _addr.sin_port = htons(port);

    if (bind(this->_tcp_socket, (struct sockaddr *)&_addr, sizeof(_addr)) == -1)
		throw (std::runtime_error("Bind failed"));
}

void	server::create_connection(int backlog) {
    if (listen(this->_tcp_socket, backlog) == -1)
		throw (std::runtime_error("Listen failed"));
}

//------Helper functions------
int    server::location_size(string_iterator it, string_iterator end) {
    for (int i = 0; it != end; it++) {
        std::string str = it->data();

        if ((int)str.find("}") != -1)
            return i;
        i++;
    }
    return 0;
}

int     server::update_request_buffer(int fd, std::string request) {
    std::map<int, std::string>::iterator it;

    it = _request_buffer.find(fd);
    if (it == _request_buffer.end()) {
        _request_buffer.insert(std::pair<int, std::string>(fd, request));
        it = _request_buffer.find(fd);
    }
    else
        it->second = it->second.append(request);
    return (valid_request(it->second));
}

int     server::valid_request(std::string request) {
    int header_size;
    int pos;

    if ((header_size = (int)request.find("\r\n")) != -1) {
        if ((pos = (int)request.find("Content-Length:")) == -1)
            return valid_;
        int content_length = ft_atoi(request.c_str() + (pos + 16));
        printf("conent length = %d\n", content_length); //remove later
        std::string body = request.substr(header_size+2); //after two linebreaks ??
        std::cout << "body = " << body << std::endl;
        if ((int)body.length() == content_length)
            return valid_;
    }
    return invalid_;
}

//------Getters------
int                             server::get_file_size(){return _max_file_size;}
int                             server::get_port(){return _port;}
std::string                     server::get_host(){return _host;}
std::string                     server::get_server_name(){return _server_name;}
std::string                     server::get_error_page(){return _error_page;}
std::vector<location_context>   server::get_location(){return _location;}
int                             server::get_tcp_socket(){return _tcp_socket;}
int                             server::get_addr_len() {return _addr_len;}
struct	sockaddr_in             server::get_addr(){return _addr;}
