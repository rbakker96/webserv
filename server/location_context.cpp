/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   location_context.cpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/30 14:07:20 by roybakker     #+#    #+#                 */
/*   Updated: 2021/03/30 14:07:20 by roybakker     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "location_context.hpp"

location_context::location_context();
location_context::location_context(location_context const &src);
location_context::~location_context();

location_context& operator=(location_context const &rhs);