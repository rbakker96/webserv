#include "file_descriptors.hpp"

file_descriptors::file_descriptors() : _max(-1) {
    FD_ZERO(&_read);
    FD_ZERO(&_write);
    FD_ZERO(&_read_buffer);
    FD_ZERO(&_write_buffer);
}

file_descriptors::~file_descriptors() {
    _time_out_monitor.clear();
}


//-------------------------------------- GENERAL functions --------------------------------------
void    file_descriptors::synchronize(file_descriptors::vector servers) {
    _read = _read_buffer;
    _write = _write_buffer;

    for (size_t index = 0; index < servers.size(); index++) {
        FD_SET(servers[index].get_tcp_socket(), &_read);
    }
}

void    file_descriptors::initialize_max(file_descriptors::vector servers) {
    for (size_t index = 0; index < servers.size(); index++) {
        if (servers[index].get_tcp_socket() > _max)
            _max = servers[index].get_tcp_socket();
    }
}

void	file_descriptors::update_max(int fd) {
    if (_max > fd)
        return;
    _max = fd;
}


//-------------------------------------- UPDATE functions --------------------------------------
void     file_descriptors::accepted_request_update(int activeFD) {
    set_read_buffer(activeFD);
    update_max(activeFD);
}

void     file_descriptors::handled_request_update(int fileFD, int activeFD, std::string content_type, std::string method) {
    clr_from_read_buffer(activeFD);
    update_max(fileFD);
    if (fileFD != -1)
        set_read_buffer(fileFD);
    else
        set_write_buffer(activeFD);
    if (content_type == "php" || method == "PUT")
        set_write_buffer(fileFD);
}

void     file_descriptors:: read_request_update(int fileFD, int activeFD) {
    if (rdy_for_writing(fileFD))
        clr_from_write_buffer(fileFD);
    clr_from_read_buffer(fileFD);
    set_write_buffer(activeFD);
}


//-------------------------------------- SET functions --------------------------------------
void     file_descriptors::set_read_buffer(int fd) {FD_SET(fd, &_read_buffer);}
void     file_descriptors::set_write_buffer(int fd) {FD_SET(fd, &_write_buffer);}


//-------------------------------------- CLR functions --------------------------------------
void     file_descriptors::clr_from_read_buffer(int fd) {FD_CLR(fd, &_read_buffer);}
void     file_descriptors::clr_from_write_buffer(int fd) {FD_CLR(fd, &_write_buffer);}


//-------------------------------------- IS_SET functions --------------------------------------
int     file_descriptors::rdy_for_reading(int fd) {
    if (fd == -1)
        return 0;
    return FD_ISSET(fd, &_read);
}
int     file_descriptors::rdy_for_writing(int fd) {
    if (fd == -1)
        return 0;
    return FD_ISSET(fd, &_write);
}


//-------------------------------------- TIME OUT functions --------------------------------------
void    file_descriptors::set_time_out(int fd) {
    struct timeval	timeval;
    long long		time;
    map_iterator    it = _time_out_monitor.find(fd);

    gettimeofday(&timeval, NULL);
    time = timeval.tv_sec;

    if (it == _time_out_monitor.end())
        _time_out_monitor.insert(std::pair<int, long long>(fd, time));
    else
        it->second = time;
}

int     file_descriptors::check_time_out(int fd, int time_out) {
    struct timeval	timeval;
    long long		time;
    map_iterator    it = _time_out_monitor.find(fd);

    gettimeofday(&timeval, NULL);
    time = timeval.tv_sec;
    if (it->second != 0 && (time - it->second) >= time_out) {
        clr_from_read_buffer(fd);
        close(fd);
        return -1;
    }
    return fd;
}


//-------------------------------------- GET functions --------------------------------------
fd_set&  file_descriptors::get_read() {return _read;}
fd_set&  file_descriptors::get_write() {return _write;}
fd_set&  file_descriptors::get_read_buffer() {return _read_buffer;}
fd_set&  file_descriptors::get_write_buffer() {return _read_buffer;}
int		 file_descriptors::get_max() {return (_max + 1);}