#ifndef WEBSERVER_CLIENT_HPP
#define WEBSERVER_CLIENT_HPP

//custom includes
#include "header_handler.hpp"

class client {
public:
    friend class webserver;

private:
    //File descriptors
    int                 _clientFD;
    int                 _fileFD;
    int 	            _cgi_inputFD;

    //Handler
    header_handler      _handler;

    //Index
    int                 _index;

public:
    client();
    client(int newFD, int client_amount);
    ~client();

    //GET functions
    int         get_clientFD();
    int         get_fileFD();
    int         get_cgi_inputFD();

    //SET functions
    void        set_clientFD(int fd);
    void        set_fileFD(int fd);
    void        set_cgi_inputFD(int fd);

};


#endif //WEBSERVER_CLIENT_HPP
