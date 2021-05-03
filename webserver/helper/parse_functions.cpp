/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_atoi.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: gbouwen <marvin@codam.nl>                    +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/28 13:03:13 by gbouwen       #+#    #+#                 */
/*   Updated: 2019/11/18 13:15:37 by gbouwen       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "helper.hpp"

std::vector<std::string>	parse_vector(const std::string& str) {

	std::vector<std::string>	vector;
	size_t						start = str.find_first_not_of(' ');
	size_t						pos = str.find_first_of(' ', start);
	std::string					tmp = str.substr(pos + 1);
	std::string					value;

	while ((pos =tmp.find_first_of(' ')) != std::string::npos) {
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


