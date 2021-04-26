/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strdup.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: gbouwen <marvin@codam.nl>                    +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/26 17:07:36 by gbouwen       #+#    #+#                 */
/*   Updated: 2021/04/26 17:10:28 by gbouwen       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "helper.hpp"

char	*ft_strdup(const char *str)
{
	int		index;
	char	*result;

	result = (char *)malloc(strlen(str) + 1);
	if (!result)
		throw std::runtime_error("Malloc failed");
	index = 0;
	while (str[index] != '\0')
	{
		result[index] = str[index];
		index++;
	}
	result[index] = '\0';
	return (result);
}
