/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   webserver.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/30 16:30:47 by roybakker     #+#    #+#                 */
/*   Updated: 2021/04/15 12:18:27 by gbouwen       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HELPER_HPP
# define HELPER_HPP

#include <string>
#include <vector>

int							ft_atoi(const char *str);
char						*ft_itoa(int n);

std::vector<std::string>	parse_vector(const std::string& str);
std::string					parse_string (const std::string& str);
int 						parse_number (const std::string& str);
void 						parse_invalid (const std::string& str);

#endif
