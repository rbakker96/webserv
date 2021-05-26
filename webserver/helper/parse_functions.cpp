/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_atoi.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: gbouwen <marvin@codam.nl>                    +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/28 13:03:13 by gbouwen       #+#    #+#                 */
/*   Updated: 2021/05/18 14:47:41 by gbouwen       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "helper.hpp"

std::vector<std::string>	parse_vector(const std::string& str) {

	std::vector<std::string>	vector;
	size_t						start = str.find_first_not_of(' ');
	size_t						pos = str.find_first_of(' ', start);
	std::string					tmp = str.substr(pos + 1);
	std::string					value;

	while ((pos = tmp.find_first_of(' ')) != std::string::npos) {
		value = tmp.substr(0, pos);
		vector.push_back(value);
		tmp = tmp.substr(pos + 1);
	}
	vector.push_back(tmp);
	return (vector);
}

std::string	parse_string (const std::string& str) {

	size_t start = str.find_first_not_of(' ');
	size_t pos = str.find_first_of(' ', start);
	return (str.substr(pos + 1));
}

int parse_number (const std::string& str) {

	size_t start = str.find_first_not_of(' ');
	size_t pos = str.find_first_of(' ', start);
	std::string temp = str.substr(pos + 1);
	return (ft_atoi(temp.c_str()));
}

void parse_invalid(const std::string& str) {
	if (str == "0")
		return;
}

vector    str_to_vector(std::string request) {
    std::vector<std::string> request_elements;
    size_t      pos = 0;
    std::string value;


    while ((int)(pos = request.find_first_of('\r')) != -1) {
        value = request.substr(0, pos);
        request_elements.push_back(value);
        request = request.substr(pos + 2);
    }
    request_elements.push_back(request);
    return request_elements;
}

std::string    read_browser_request(int fd) {
    std::string tmp;
    char        buff[3000];
    int         ret = 1;

    while (ret > 0) {
        if ((ret = read(fd, buff, 3000)) != -1)
            tmp.append(buff, ret);
        if (ret < 3000)
            break;
    }
    if (ret == -1)
        throw (std::string("Read browser request failed"));
    return tmp;
}

