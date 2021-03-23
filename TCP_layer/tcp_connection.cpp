/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tcp_layer.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: roybakker <roybakker@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/16 16:29:05 by roybakker     #+#    #+#                 */
/*   Updated: 2021/03/16 16:29:05 by roybakker     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "tcp_connection.hpp"
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


tcp_connection::tcp_connection() {}

tcp_connection::tcp_connection(tcp_connection const &src){}

tcp_connection::~tcp_connection(){}

int tcp_connection::create_socket() {
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("cannot create tcp socket");
        return 1;
    }
    return 0;
}

int tcp_connection::bind_socket_addrs(int port) {
    memset((char *)&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if (bind(server_fd,(struct sockaddr *)&addr,sizeof(addr)) < 0) {
        perror("bind failed");
        return 1;
    }
    return 0;
}

int tcp_connection::create_connection(int backlog) {
    if (listen(server_fd, backlog) < 0) {
        perror("listen failed");
        return 1;
    }
    if ((tcp_socket = accept(server_fd, (struct sockaddr *)&addr, (socklen_t*)&addr_len))<0) {
        perror("accept failed");
        return 1;
    }
    return 0;
}

int tcp_connection::response(char *hello) {
    while (1) {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((tcp_socket = accept(server_fd, (struct sockaddr *) &addr, (socklen_t *) & addr_len)) < 0) {
            perror("accept failure");
            return 1;
        }

        char buffer[30000] = {0};
        long valread = read(tcp_socket, buffer, 30000);
        printf("%s\n", buffer);
        write(tcp_socket, hello, strlen(hello));
        printf("------------------Hello message sent-------------------\n");
        close(tcp_socket);
    }
    return 0;
}
