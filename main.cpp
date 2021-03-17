/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/16 16:23:09 by roybakker     #+#    #+#                 */
/*   Updated: 2021/03/16 16:23:09 by roybakker     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "TCP_layer/tcp_connection.hpp"

int main() {
    tcp_connection tcp;

    while (1) {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((tcp.tcp_socket = accept(tcp.server_fd, (struct sockaddr *)&tcp.addr, (socklen_t*)&tcp.addr_len))<0) {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        char buffer[30000] = {0};
        int valread = read( tcp.tcp_socket , buffer, 30000);
        printf("%s\n",buffer );
        write(tcp.tcp_socket , hello , strlen(hello));
        printf("------------------Hello message sent-------------------\n");
        close(tcp.tcp_socket);
    }
}

