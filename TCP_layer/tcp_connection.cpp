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

tcp_connection::tcp_connection() {
    //Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("cannot create tcp socket");
        return 0; //exit or return ?
    }

    //Set addr values
    memset((char *)&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8080);

    //Bind socket to an address
    if (bind(server_fd,(struct sockaddr *)&addr,sizeof(addr)) < 0) {
        perror("bind failed");
        return 0; //exit or return ?
    }

    //Prepare for incomming connection
    if (listen(server_fd, 3) < 0) { //backlog set to 3, change later ?!
        perror("In listen");
        exit(1); //exit or return ?
    }
    if ((tcp_socket = accept(server_fd, (struct sockaddr *)&addr, (socklen_t*)&addr_len))<0) {
        perror("In accept");
        exit(1); // exit or return ?
    }
}

tcp_connection::tcp_connection(tcp_connection const &src){}

tcp_connection::~tcp_connection(){}