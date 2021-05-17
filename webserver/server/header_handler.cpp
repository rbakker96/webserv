/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   header_handler.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: gbouwen <marvin@codam.nl>                    +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/03 12:34:40 by gbouwen       #+#    #+#                 */
/*   Updated: 2021/05/13 13:50:24 by gbouwen       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "header_handler.hpp"
#include "../helper/helper.hpp"

header_handler::header_handler(): _index(0), _status(okay_), _status_phrases(), _content_length(0), _content_type("Content-Type: text/"),
                                  _content_language("en"), _content_location(), _allow(), _method(), _file_location(),
                                  _uri_location(), _protocol(), _requested_host(), _user_agent(),
                                  _accept_language(), _authorization(), _referer(), _body(), _response_file()	{

	_status_phrases.insert (pair(200, "OK"));
	_status_phrases.insert (pair(201, "Created"));
	_status_phrases.insert (pair(204, "No Content"));
	_status_phrases.insert (pair(400, "Bad Request"));
	_status_phrases.insert (pair(401, "Unauthorized"));
	_status_phrases.insert (pair(403, "Forbidden"));
	_status_phrases.insert (pair(404, "Not Found"));
	_status_phrases.insert (pair(405, "Method Not Allowed"));
	_status_phrases.insert (pair(413, "Payload Too Large"));
}

header_handler::~header_handler(){
    reset_handler();
    _status_phrases.clear();
}


//-------------------------------------- PARSE functions --------------------------------------
void        header_handler::parse_request(int fd, header_handler::map request_buffer) {
    map_iterator request                        = request_buffer.find(fd);
    vector request_elements                     = str_to_vector(request->second);
    parse parse_array[10]                       = { &header_handler::parse_requested_host,
                                                    &header_handler::parse_user_agent,
                                                    &header_handler::parse_language,
                                                    &header_handler::parse_authorization,
                                                    &header_handler::parse_referer,
                                                    &header_handler::parse_content_length,
                                                    &header_handler::parse_content_type,
                                                    &header_handler::parse_content_language,
                                                    &header_handler::parse_content_location,
                                                    &header_handler::invalid_argument};

    reset_handler();
    parse_first_line(*request_elements.begin());
    parse_body(request->second);
    for (vector_iterator it = request_elements.begin(); it != request_elements.end(); it++) {
        int request_value = identify_request_value(*it);
        parse function = parse_array[request_value];
        (this->*function)(*it);
    }

    print_request(request->second); //DEBUG
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
    else if (str.find("Content-Length:") != std::string::npos)
        return content_length_;
    else if (str.find("Content-Type:") != std::string::npos)
        return content_type_;
    else if (str.find("Content-Language:") != std::string::npos)
        return content_language_;
    else if (str.find("Content-Location:") != std::string::npos)
        return content_location_;
    return unknown_;
}

void        header_handler::parse_first_line(const std::string &str) {
	int		index;
    size_t	start = 0;
    size_t	end = str.find_first_of(' ', start);
    _method = str.substr(start, end - start);

    start = end + 1;
    end = str.find_first_of(' ', start);
	if ((index = str.find('?', start)) != -1)
	{
		_uri_location = str.substr(start, index - start);
		index++;
		_body = str.substr(index, end - index);
	}
	else
		_uri_location = str.substr(start, end - start);
    _protocol = str.substr(end + 1);
}

void        header_handler::parse_body(const std::string &str) {
    int header_size;

    if ((header_size = (int)str.find("\r\n\r\n")) != -1) {
        if (str.find("Content-Length:") != std::string::npos) {
            _body = str.substr(header_size + 4);
        }
        else if (str.find("chunked") != std::string::npos) {
            std::string body = str.substr(header_size + 4);
            _body = body.substr(0, body.length() - 4);
        }
    }
}

void        header_handler::parse_requested_host(const std::string &str) {_requested_host = parse_string(str);}
void        header_handler::parse_user_agent(const std::string &str) {_user_agent = parse_string(str);}
void        header_handler::parse_language(const std::string &str) {_accept_language = parse_string(str);}
void        header_handler::parse_authorization(const std::string &str) {_authorization = parse_string(str);}
void        header_handler::parse_referer(const std::string &str) {_referer = parse_string(str);}
void        header_handler::parse_content_length(const std::string &str) {_content_length = parse_number(str);}
void        header_handler::parse_content_type(const std::string &str) {_content_type = parse_string(str);}
void        header_handler::parse_content_language(const std::string &str) {_content_language = parse_string(str);}
void        header_handler::parse_content_location(const std::string &str) {_content_location = parse_string(str);}
void        header_handler::invalid_argument(const std::string &str) {parse_invalid(str);}


//-------------------------------------- HANDLE functions --------------------------------------
int        header_handler::handle_request(std::string cgi_file_types, header_handler::location_vector location_blocks, std::string error_page) {
	struct  stat stats;
	int		fd = unused_;

    verify_file_location(location_blocks, error_page);
	verify_method();
	if (_status < error_code_)
    {
        if (_method == "PUT")
            fd = put_request();
		else if (stat(_file_location.c_str(), &stats) == -1)
			_status = not_found_; // update the status code after the version is stable
		else if (!(stats.st_mode & S_IRUSR))
			_status = forbidden_; // update the status code after the version is stable
        else if (cgi_file_types.find(verify_content_type()) != std::string::npos)
			return cgi_request();
        else if ((fd = open(&_file_location[0], O_RDONLY)) == -1)
			throw std::runtime_error("Open failed");
    }

    if (_status >= error_code_){
        _file_location = error_page.append(ft_itoa(_status));
        _file_location.append(".html");
        if ((fd = open(&_file_location[0], O_RDONLY)) == -1)
            throw std::runtime_error("Open failed");
    }
    if (fd != -1)
        fcntl(fd, F_SETFL, O_NONBLOCK);
    return fd;
}

std::string	header_handler::get_referer_part()
{
	int			start;
	std::string	result;

	if (_referer.empty())
		return ("");
	start = _referer.find(_requested_host);
	start += _requested_host.length();
	result = _referer.substr(start, std::string::npos);
	if (result.empty())
		return ("");
	return (result);
}

int	check_if_file(std::string file_location)
{
	std::vector<std::string>	extensions;
    extensions.push_back(".html");
    extensions.push_back(".php");
    extensions.push_back(".css");
    extensions.push_back(".ico");
    extensions.push_back(".png");
	extensions.push_back(".bla");
	extensions.push_back(".bad_extension");
	extensions.push_back(".pouic");
	extensions.push_back(".pouac"); // check directory first and then file

	// search for entire len after . and strcmp
    for (header_handler::vector_iterator it = extensions.begin(); it != extensions.end(); it++) {
        if (file_location.find(*it) != std::string::npos) {
			return (1);
		}
    }
    return 0;
}

int	compare_directory(std::string file_location, std::string location_context)
{
	std::string	parent_directory = file_location;

	if (file_location == location_context)
		return (1);
	while (1)
	{
		int	end = parent_directory.find_last_of('/');
		if (end == 0)
			break ;
		parent_directory = file_location.substr(0, end);
		if (parent_directory == location_context)
			return (1);
	}
	return (0);
}

int	compare_file(std::string file_location, std::string location_context)
{
	std::string	parent_directory = file_location;

	while (1)
	{
		int	end = parent_directory.find_last_of('/');
		if (end == 0 && (check_if_file(parent_directory)))
			return (1);
		if (end == -1)
			break ;
		parent_directory = file_location.substr(0, end);
		if (parent_directory == location_context)
			return (1);
	}
	return (0);
}

int			header_handler::match_location_block(header_handler::location_vector location_blocks, std::string file_location)
{
	std::string	location_context;
	std::string	referer_location = get_referer_part();

	for (size_t index = 0; index < location_blocks.size(); index++)
	{
		location_context = location_blocks[index].get_location_context();
		std::cout << "LOCATION_CONTEXT : " << location_context << std::endl;
		std::cout << "FILE_LOCATION : " << file_location << std::endl;
		if (!_referer.empty() && check_if_file(referer_location) == 0 && location_context == referer_location)
			return (index);
		else if (_referer.empty() || check_if_file(referer_location) == 1)
		{
			if (check_if_file(file_location) == 1 && compare_file(file_location, location_context) == 1)
				return (index);
			else if (check_if_file(file_location) == 0 && compare_directory(file_location, location_context) == 1)
				return (index);
		}
	}
	return (-1);
}

std::string	get_subdirectories(std::string str)
{
	int			start_index;
	int			end_index;
	std::string	subdir;

	start_index = str.find_first_of('/', 1);
	if (start_index == -1)
		return ("");
	end_index = str.find_last_of('/', std::string::npos);
	if (start_index == end_index)
		subdir = str.substr(start_index, std::string::npos);
	else
		subdir = str.substr(start_index, end_index - start_index);
	if (check_if_file(subdir))
		return ("");
	return (subdir);
}

std::string	get_subdirectories_referer(std::string str)
{
	if (check_if_file(str))
		return ("");
	else
		return (get_subdirectories(str));
}

std::string	get_file(location_context location_block, std::string file_location, std::string correct_location)
{
	int	start_index;

	if (check_if_file(file_location))
	{
		start_index = file_location.find_last_of('/', std::string::npos);
		return (file_location.substr(start_index, std::string::npos));
	}
	else
	{
		struct stat	s;
		std::string	temp = correct_location;

		temp.append(location_block.get_index());
		if (stat(temp.c_str(), &s) == -1 && location_block.get_autoindex())
			return ("/index.php");
		return (location_block.get_index());
	}
}

std::string	header_handler::generate_error_page_location(std::string error_page)
{
	int pos = _uri_location.find_last_of('/');
	std::string temp = _uri_location.substr(pos+1);
	return (error_page.append(temp));
}

void        header_handler::verify_file_location(header_handler::location_vector location_blocks, std::string error_page) {
	int 		index = match_location_block(location_blocks, _uri_location);
	std::string	correct_location;
	std::string	referer_part = get_referer_part();
	struct stat	s;

	std::cout << "INDEX : " << index << std::endl;
	if (index == -1)
		correct_location = generate_error_page_location(error_page);
	else
	{
        _allow = location_blocks[index].get_method();
		_location_block_root = location_blocks[index].get_root();
		correct_location = _location_block_root;
		correct_location.append(get_subdirectories_referer(referer_part));
		std::cout << "0 correct_location : " << correct_location << std::endl;
		correct_location.append(get_subdirectories(_uri_location));
		std::cout << "1 correct_location : " << correct_location << std::endl;
		correct_location.append(get_file(location_blocks[index], _uri_location, correct_location));
		std::cout << "2 correct_location : " << correct_location << std::endl;
	}
	if (stat(correct_location.c_str(), &s) == -1)
		correct_location = generate_error_page_location(error_page);
	_file_location = correct_location;
}

void 		header_handler::verify_method()
{
	if (_method == "POST" && verify_content_type() == "bla")
		return;
	if (_method == "POST" && get_body().empty())
		_status = method_not_allowed_;
	if (!_allow.empty())
	{
		for (vector_iterator it = _allow.begin(); it != _allow.end(); it++)
		{
			if (*it == _method)
				return;
		}
	}
	_status = method_not_allowed_;
}

// look into the other content type
std::string     header_handler::verify_content_type() {
    vector extensions;
    extensions.push_back("html");
    extensions.push_back("php");
    extensions.push_back("css");
    extensions.push_back("ico");
    extensions.push_back("png");
    extensions.push_back("bla");

    for (vector_iterator it = extensions.begin(); it != extensions.end(); it++) {
        if (_file_location.find(*it) != std::string::npos) {
            return *it;
        }
    }
    return "folder";
}

int			header_handler::cgi_request()
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

int     	header_handler::put_request()
{
    int fd = unused_;
	struct  stat stats;
    std::string folder = "server_files/www/downloads/";
    std::string file = _file_location.substr(_file_location.find_last_of('/') + 1);
    std::string put_file = folder.append(file);

	if (stat(put_file.c_str(), &stats) != -1) {
        _status = no_content_;
        fd = open(&put_file[0], O_RDWR | O_TRUNC, S_IRWXU);
    }
	else {
        _status = created_;
        fd = open(&put_file[0], O_RDWR | O_CREAT, S_IRWXU);
    }
    if (fd == -1)
        throw std::runtime_error("Open failed");
    return fd;
}

void        header_handler::write_put_file(int file_fd) {
    if ((write(file_fd, _body.c_str(), _body.length())) == -1)
        throw std::runtime_error("Write failed");
}


//-------------------------------------- CGI functions --------------------------------------
std::string	get_correct_directory(std::string &file_location)
{
	int			last_index = file_location.find_last_of("/", std::string::npos);
	std::string	result = file_location.substr(0, last_index);

	return (result);
}

// error checking if execve fails
void header_handler::execute_cgi(int fileFD, std::string server_name, int server_port)
{
	pid_t	pid;
	int 	status = 0;

	pid = fork();
	if (pid == -1)
		throw std::runtime_error("Fork failed");
	if (pid == 0)
	{
		char	**args = create_cgi_args(); // error management
		char 	**envp = create_cgi_envp(server_name, server_port); // error management

		dup2(fileFD, STDOUT_FILENO);
		execve(args[0], args, envp);
		exit(EXIT_FAILURE);
	}
	else
		waitpid(pid, &status, 0);
	std::cout << RED << "LEFT CGI\n" << RESET;

}

std::string	get_location_without_root(std::string &file_location)
{
	std::string	result;

	result = file_location.substr(file_location.find_last_of("/") + 1);
	return (result);
}

char	**header_handler::create_cgi_args()
{
	char	**args = new char *[3];
	char 	server_root[PATH_MAX];

	getcwd(server_root, (size_t)PATH_MAX);
	if (_file_location.find(".php") != std::string::npos)
		args[0] = ft_strdup("/usr/bin/php");
	else if (_file_location.find(".bla") != std::string::npos)
		args[0] = ft_strjoin(server_root, "/tester_executables/cgi_tester");

	char *tmp = ft_strjoin(server_root, "/");
	args[1] = ft_strjoin(tmp, _file_location.c_str());
	args[2] = NULL;

	std::cout << RED << "args[0]: "<< args[0] << std::endl;
	std::cout << RED << "args[1]: "<< args[1] << RESET << std::endl;
	std::cout << RED << "_uri_location: " << _uri_location << std::endl << RESET;
	std::cout << RED << "_file_location: "<< _file_location << std::endl << RESET;
	return args;
}

char **header_handler::create_cgi_envp(const std::string &server_name, int server_port)
{
	// for later
	// AUTH_TYPE, REMOTE_ADDR, REMOTE_IDENT, REMOTE_USER
	std::cout << "ENTER ENVP\n" << RESET;

	vector	cgi_envps;
	char 	server_root[PATH_MAX];
	getcwd(server_root, (size_t)PATH_MAX); // check error

	cgi_envps.push_back((std::string)"AUTH_TYPE=");
	cgi_envps.push_back(((std::string)"CONTENT_LENGTH=").append(ft_itoa(get_content_length()))); //leaks ??
	cgi_envps.push_back(((std::string)"CONTENT_TYPE=").append(get_content_type()));
	cgi_envps.push_back((std::string)"GATEWAY_INTERFACE=CGI/1.1");
	cgi_envps.push_back((std::string)"HTTP_REFERER=");
	cgi_envps.push_back(((std::string)"PATH_INFO=").append(_uri_location));
	cgi_envps.push_back(((((std::string)"PATH_TRANSLATED=").append(server_root)).append("/")).append(_location_block_root).append(_uri_location));
	cgi_envps.push_back(((std::string)"QUERY_STRING="));
	cgi_envps.push_back(((std::string)"REDIRECT_STATUS=true"));
	cgi_envps.push_back(((std::string)"REMOTE_IDENT="));
	cgi_envps.push_back(((std::string)"REMOTE_USER="));
	cgi_envps.push_back(((std::string)"REQUEST_METHOD=").append(get_method()));
	cgi_envps.push_back(((std::string)"REQUEST_URI=").append(_uri_location));
	cgi_envps.push_back(((((std::string)"SCRIPT_FILENAME=").append(server_root)).append("/")).append(_file_location));
	cgi_envps.push_back(((std::string)"SCRIPT_NAME=").append(get_location_without_root(_uri_location)));
	cgi_envps.push_back(((std::string)"SERVER_NAME=").append(server_name));
	cgi_envps.push_back(((std::string)"SERVER_PORT=").append(ft_itoa(server_port)));
	cgi_envps.push_back(((std::string)"SERVER_PROTOCOL=").append(get_protocol()));
	cgi_envps.push_back((std::string)"SERVER_SOFTWARE=HTTP 1.1");

	char 	**envp = new char *[cgi_envps.size() + 1];
	int		i = 0;

	for (vector_iterator it = cgi_envps.begin(); it != cgi_envps.end(); it++) {
		envp[i] = ft_strdup((*it).c_str()); // error check on ft_strdup failure
		std::cout << CYAN << envp[i] << RESET << std::endl;
		i++;
	}
	std::cout << std::endl;
	envp[cgi_envps.size()] = NULL;
	std::cout << "LEFT ENVP\n" << RESET;
	return envp;
}


//-------------------------------------- RESPONSE functions --------------------------------------
void        header_handler::read_requested_file(int fd) {
    char    buff[3000];
    int     ret = 1;

    lseek(fd, 0, SEEK_SET);
    while (ret > 0) {
        ret = read(fd, buff, 3000);
        _response_file.append(buff, ret);
        if (ret < 3000)
            break;
    }
    if (ret == -1)
        throw std::runtime_error("Read failed");
}

void    header_handler::send_response(int activeFD, int fileFD, std::string server_name) {
    response response;

    response.generate_status_line(_protocol, _status, _status_phrases);
    response.generate_content_length(_response_file);
    response.generate_content_type(verify_content_type());
    response.generate_last_modified(fileFD);
    response.generate_date();
    response.generate_server_name(server_name);
    if (_status == method_not_allowed_)
        response.generate_allowe(_allow);
    response.generate_connection_close();
    response.close_header_section();

    response.write_response_to_browser(activeFD, _response_file, _method);

    print_response(response.get_response()); //DEBUG
    reset_handler();
}


//-------------------------------------- RESET functions --------------------------------------
void    header_handler::reset_handler() {
    _status = 200;
    _content_length = 0;
    _method.clear();
    _file_location.clear();
    _protocol.clear();
    _response_file.clear();
    _requested_host.clear();
    _user_agent.clear();
    _accept_language.clear();
    _authorization.clear();
    _referer.clear();
    _body.clear();
    _content_type.clear();
    _content_language = "en";
    _content_location.clear();
}


//-------------------------------------- GET functions --------------------------------------
int				        header_handler::get_index() {return _index;}
int                     header_handler::get_status() {return _status;}
int                     header_handler::get_content_length() {return _content_length;}
std::string             header_handler::get_content_type() {return _content_type;}
std::string             header_handler::get_content_language() {return _content_language;}
std::string             header_handler::get_content_location() {return _content_location;}
header_handler::vector	header_handler::get_allow() {return _allow;}
std::string	            header_handler::get_response_file() {return (_response_file);}
std::string             header_handler::get_method() {return _method;}
std::string             header_handler::get_file_location() {return _file_location;}
std::string             header_handler::get_uri_location() {return _uri_location;}
std::string				header_handler::get_location_block_root() { return _location_block_root; }
std::string             header_handler::get_protocol() {return _protocol;}
std::string             header_handler::get_requested_host() {return _requested_host;}
std::string             header_handler::get_user_agent() {return _user_agent;}
std::string             header_handler::get_accept_language() {return _accept_language;}
std::string             header_handler::get_authorization() {return _authorization;}
std::string             header_handler::get_referer() {return _referer;}
std::string             header_handler::get_body() {return _body;}


//-------------------------------------- SET functions --------------------------------------
void			        header_handler::set_index(int index) { _index = index;}


// // // // // // // // // // // // //  DEBUG functions // // // // // // // // // // // // //
void header_handler::print_request(std::string request) {
    std::cout << "-----------\n" << YELLOW << "REQUEST BUFFER: \n" << request << RESET << std::endl;

    std::cout << YELLOW << "------------- REQUEST -------------\n";
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

    std::cout << "Entity headers\n";
    std::cout << "  Content length = " << get_content_length() << std::endl;
    std::cout << "  Content type = " << get_content_type() << std::endl;
    std::cout << "  Content language = " << get_content_language() << std::endl;
    std::cout << "  Content location = " << get_content_location() << std::endl;
    std::cout << "------------- END REQUEST -------------\n\n" << RESET;
}

void    header_handler::print_response(std::string response) {
    std::cout << MAGENTA << "\n------------- RESPONSE -------------\n";
    std::cout << response << RESET << std::endl;
}
