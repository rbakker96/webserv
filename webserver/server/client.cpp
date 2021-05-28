#include "client.hpp"

client::client() : _clientFD(-1), _fileFD(-1), _cgi_inputFD(-1), _authorization_status(false), _handler(), _index(0){}

client::client(int newFD, int client_amount) : _clientFD(newFD), _fileFD(-1), _cgi_inputFD(-1), _authorization_status(false), _handler(), _index(client_amount) {}

client::~client() {}

//-------------------------------------- GET functions --------------------------------------
int         client::get_clientFD() {return _clientFD;}
int         client::get_fileFD() {return _fileFD;}
int         client::get_cgi_inputFD() {return _cgi_inputFD;}
bool        client::get_authorization_status() {return _authorization_status;}
bool		client::get_time_out_check() { return _time_out_check; }

//-------------------------------------- SET functions --------------------------------------
void        client::set_clientFD(int fd) {_clientFD = fd;}
void        client::set_fileFD(int fd) {_fileFD = fd;}
void        client::set_cgi_inputFD(int fd) {_cgi_inputFD = fd;}
void        client::set_authorization_status(bool status) {_authorization_status = status;}
void		client::set_time_out_check(bool time_out_check) { _time_out_check = time_out_check; }
