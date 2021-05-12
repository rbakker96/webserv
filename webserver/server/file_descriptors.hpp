
#ifndef WEBSERVER_FILE_DESCRIPTORS_HPP
#define WEBSERVER_FILE_DESCRIPTORS_HPP

//general includes
#include <sys/time.h>
#include <sys/socket.h>
#include <vector>

//custom includes
#include "server.hpp"

class file_descriptors {
public:
    typedef     std::vector<server>                   vector;
    typedef     std::map<int, long long>::iterator    map_iterator;

private:
    fd_set	                    _read;
    fd_set                      _write;
    fd_set	                    _read_buffer;
    fd_set                      _write_buffer;
    int			                _max;

    std::map<int, long long>    _time_out_monitor;

public:
    file_descriptors();
    ~file_descriptors();

    void    	synchronize(vector servers);
    void    	initialize_max(vector servers);
    void		update_max(int fd);

    //UPDATE functions
    void        accepted_request_update(int activeFD);
    void        handled_request_update(int fileFD, int activeFD, std::string content_type, std::string method);
    void        read_request_update(int fileFD, int activeFD);

    //SET functions
    void        set_read_buffer(int fd);
    void        set_write_buffer(int fd);

    //CLR functions
    void        clr_from_read_buffer(int fd);
    void        clr_from_write_buffer(int fd);

    //IS_SET functions
    int         rdy_for_reading(int fd);
    int         rdy_for_writing(int fd);

    //TIME OUT functions
    void        set_time_out(int fd);
    int         check_time_out(int fd, int time_out);

    //Getter
    fd_set&     get_read();
    fd_set&     get_write();
    fd_set&     get_read_buffer();
    fd_set&     get_write_buffer();
    int			get_max();
};

#endif //WEBSERVER_FILE_DESCRIPTORS_HPP