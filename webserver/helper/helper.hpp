/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   webserver.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/30 16:30:47 by roybakker     #+#    #+#                 */
/*   Updated: 2021/05/18 14:56:20 by gbouwen       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HELPER_HPP
# define HELPER_HPP

#include <string>
#include <vector>
#include <cstring>
#include <stdexcept>
#include <cstdlib> // for malloc && free && exit
#include <unistd.h>
#include <iostream>
#include <sys/stat.h>

#include "../server/location_context.hpp"

# define RESET			"\033[0m"
# define CYAN			"\033[36m"

typedef     std::vector<std::string>                vector;
typedef     std::vector<std::string>::iterator      vector_iterator;
enum        helper_values{valid_ = 7, invalid_ = 8 };

//check_functions
int                         check_server_block(std::vector<std::string> server_block);
int                         location_block_size(vector_iterator it, vector_iterator end);
int                         hex_to_dec(std::string str, int base);

//lib functions
int							ft_atoi(const char *str);
char						*ft_itoa(int n);
char						*ft_strdup(const char *str);
char						*ft_strjoin(char const *s1, char const *s2);

//parse functions
std::vector<std::string>	parse_vector(const std::string& str);
std::string					parse_string(const std::string& str);
int 						parse_number(const std::string& str);
void 						parse_invalid(const std::string& str);
vector                      str_to_vector(std::string request);
std::string                 read_browser_request(int fd);

//signal handler function
void						signal_handler(int signal);

//verify file location functions
std::string					remove_duplicate_forward_slashes(std::string location);
std::string					get_file(location_context location_block, std::string location);
std::string					skip_first_directory(std::string uri_location);
std::string					get_first_directory(std::string uri_location);

#endif
