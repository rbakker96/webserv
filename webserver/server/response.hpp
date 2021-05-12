#ifndef WEBSERVER_RESPONSE_HPP
#define WEBSERVER_RESPONSE_HPP

//General includes
#include <string>
#include <map>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>

class response {
public:
    typedef		std::map<int, std::string>          map;
    typedef		std::vector<std::string>			vector;
    typedef		std::vector<std::string>::iterator	vector_iterator;

private:
    std::string     _response;

public:
    //GENERATE functions
    void			generate_status_line(std::string protocol, int status, response::map status_phrases);
    void			generate_content_length(std::string requested_file);
    void			generate_content_type(std::string type);
    void			generate_last_modified(int file_fd);
    void			generate_date();
    void			generate_server_name(std::string server_name);
    void			generate_allowed_methods_config(vector allowed_methods);
    void 			generate_connection_close();
    void            close_header_section();

    //SEND functions
    void            write_response_to_browser(int browser_socket, std::string response_file, std::string method);

    //Debug
    void            print_response();
};

#endif //WEBSERVER_RESPONSE_HPP
