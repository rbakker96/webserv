/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/30 13:54:06 by roybakker     #+#    #+#                 */
/*   Updated: 2021/04/15 11:41:10 by gbouwen       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "../helper/helper.hpp"
#include <stdio.h> //REMOVE LATER

server::server() : _io_fd(-1), _file_fd(-1), _max_file_size(0), _port(0), _host(), _server_name(), _error_page(), _location_blocks(), _tcp_socket(0),
                    _addr_len(0), _addr(), _request_buffer(), _handler() {}
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
    for (vector_iterator it = server_config.begin(); it != server_config.end(); it++) {
        int config_id = identify_server_value(*it);
        if (config_id == location_) {
            location.configure_location_context(it, (it + location_size(it, server_config.end())));
            _location_blocks.push_back(location);
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
    _location_blocks.clear();
}

int     server::identify_server_value(const std::string& str) {
    if (str.find("port") != std::string::npos)
        return port_;
    else if (str.find("host") != std::string::npos)
        return host_;
    else if (str.find("server_name") != std::string::npos)
        return server_name_;
    else if (str.find("error_page") != std::string::npos)
        return error_page_;
    else if (str.find("max_file_size") != std::string::npos)
        return max_file_size_;
    else if (str.find("location") != std::string::npos)
        return location_;
    return unknown_;
}

void    server::configure_port(const std::string& str) {_port = parse_number(str);}
void    server::configure_host(const std::string& str) {_host = parse_string(str);}
void    server::configure_server_name(const std::string& str) {_server_name = parse_string(str);}
void    server::configure_error_page(const std::string& str) {_error_page = parse_string(str);}
void    server::configure_max_file_size(const std::string& str) {_max_file_size = parse_number(str);}
void    server::invalid_element(const std::string& str) {parse_invalid(str);}


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
int    server::location_size(vector_iterator it, vector_iterator end) {
    for (int i = 0; it != end; it++) {
        std::string str = it->data();

        if ((int)str.find("}") != -1)
            return i;
        i++;
    }
    return 0;
}

int     server::update_request_buffer(int fd, const std::string& request) {
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

void    server::clear_handled_request(int used_fd){
    map_iterator request = _request_buffer.find(used_fd);

    _request_buffer.erase(request);
}

int     server::valid_request(const std::string& request) {
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
int                             server::get_file_size() {return _max_file_size;}
int                             server::get_port() {return _port;}
std::string                     server::get_host(){return _host;}
std::string                     server::get_server_name(){return _server_name;}
std::string                     server::get_error_page(){return _error_page;}
std::vector<location_context>   server::get_location_blocks(){return _location_blocks;}
int                             server::get_tcp_socket() {return _tcp_socket;}
int                             server::get_addr_len() {return _addr_len;}
struct	sockaddr_in             server::get_addr(){return _addr;}
