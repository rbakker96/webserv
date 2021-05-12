#include "response.hpp"
#include "../helper/helper.hpp"

response::response() {}
response::~response() {
    _response.clear();
}


//-------------------------------------- GENERATE functions --------------------------------------
void	response::generate_status_line(std::string protocol, int status, response::map status_phrases) {
    std::string status_line = protocol;
    char *status_str = ft_itoa(status);

    status_line.append(" ");
    status_line.append(status_str);
    status_line.append(" ");
    status_line.append(status_phrases[status]);
    status_line.append("\r\n");
    free(status_str);

    _response.append(status_line);
}

void	response::generate_content_length(std::string requested_file) {
    std::string	content_length = "Content-Length: ";

    content_length.append(ft_itoa(requested_file.size()));
    content_length.append("\r\n");

    _response.append(content_length);
}

void	response::generate_content_type(std::string type) {
    std::string	content_type_header = "Content-Type: ";
    std::string content_type = type;

    if (content_type.compare("php") == 0){
        content_type = "html";
    }
    if (content_type.compare("html") == 0 || content_type.compare("css") == 0)
        content_type_header.append("text/");
    else if (content_type.compare("png") == 0)
        content_type_header.append("image/");
    content_type_header.append(content_type);
    content_type_header.append("\r\n");

    _response.append(content_type_header);
}

void	response::generate_last_modified(int fileFD) {
    std::string	last_modified = "Last-Modified: ";
    struct stat	stat;
    struct tm	*info;
    char		timestamp[36];

    fstat(fileFD, &stat);
    info = localtime(&stat.st_mtime);
    strftime(timestamp, 36, "%a, %d %h %Y %H:%M:%S GMT", info);
    last_modified.append(timestamp);
    last_modified.append("\r\n");

    _response.append(last_modified);
}

void	response::generate_date()
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

    _response.append(date);
}

void	response::generate_server_name(std::string server_name) {
    std::string server_header = "Server: ";

    server_header.append(server_name);
    server_header.append("\r\n");

    _response.append(server_header);
}

void	response::generate_allowe(response::vector allowed_methods) {
    std::string allow_header = "Allow: ";

    for (vector_iterator it = allowed_methods.begin(); it != allowed_methods.end(); it++){
        allow_header.append(*it);
        if (it + 1 != allowed_methods.end())
            allow_header.append(", ");
    }
    allow_header.append("\r\n");

    _response.append(allow_header);
}

void	response::generate_connection_close() {
    _response.append("Connection: close\r\n");
}

void    response::close_header_section() {
    _response.append("\r\n");
}


//-------------------------------------- SEND functions --------------------------------------
void    response::write_response_to_browser(int browser_socket, std::string response_file, std::string method) {
    write(browser_socket, _response.c_str(), _response.size());
	if (method != "HEAD")
	    write(browser_socket, response_file.c_str(), response_file.size());
}


//-------------------------------------- GET functions --------------------------------------
std::string response::get_response() {return _response;}