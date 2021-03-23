#include "tcp_connection.hpp"
#include <unistd.h>

int     main() {
    tcp_connection connect;

    if (connect.create_socket() ||
        connect.bind_socket_addrs(8080) ||
        connect.create_connection(10) ||
        connect.response()) {
        write(1, "fail\n", 5);
    }
    return 0;
}
