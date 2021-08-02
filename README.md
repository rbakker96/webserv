<p align="center">
  <img src="https://github.com/rbakker96/images/blob/master/codam_logo.png">
</p>

# Webserv
***This project is here to make you write your HTTP server. You will be able to test it with a real browser. HTTP is one of the most used protocols on the internet. Knowing its arcane will be useful, even if you won’t be working on a website***

---

### Project summary
> 5th ring Codam project

The Hypertext Transfer Protocol (HTTP) is an application protocol for distributed, collaborative, hypermedia information systems. HTTP is the foundation of data communication for the World Wide Web, where hypertext documents include hyperlinks to other resources that the user can easily access, for example by a mouse click or by tapping the screen in a web browser. HTTP was developed to facilitate hypertext and the World Wide Web. The primary function of a web server is to store, process, and deliver web pages to clients. The communication between client and server takes place using the Hypertext Transfer Protocol (HTTP). Pages delivered are most frequently HTML documents, which may include images, style sheets, and scripts in addition to the text content.

---

### Technical considerations
> The project needs to comply with the following rules/functionalities.

- You can use every macro and define like FD_SET, FD_CLR, FD_ISSET, FD_ZERO (understanding what they do and how they do it is very useful.)
- You must write an HTTP server in C++ 98.
- If you need more C functions, you can use them but always prefer C++.
- The C++ standard must be C++ 98. Your project must compile with it.
- No external library, no Boost, etc...
- Try to always use the most "C++" code possible (for example use <cstring> over <string.h>).
- Your server must be compatible with the web browser of your choice.
- We will consider that Nginx is HTTP 1.1 compliant and may be used to compare headers and answer behaviors.
- In the subject and the scale we will mention poll but you can use equivalent like select, kqueue, epoll.
- It must be non-blocking and use only 1 poll (or equivalent) for all the IO betweenthe client and the server (listens includes).
- poll (or equivalent) should check read and write at the same time.
- Your server should never block and the client should be bounce properly if necessary.
- You should never do a read operation or a write operation without going through poll (or equivalent).
- Checking the value of errno is strictly forbidden after a read or a write operation.
- A request to your server should never hang forever.
- You server should have default error pages if none are provided.
- Your program should not leak and should never crash, (even when out of memory if all the initialization is done)
- You can’t use fork for something else than CGI (like php or python etc...)
- You can’t execve another webserver...
- Your program should have a config file in argument or use a default path.
- You don’t need to use poll (or equivalent) before reading your config file.
- You should be able to serve a fully static website.
- Client should be able to upload files.
- Your HTTP response status codes must be accurate.
- You need at least GET, POST, and DELETE methods.
- Stress tests your server it must stay available at all cost.
- Your server can listen on multiple ports (See config file).

---

### Usage
> Run the following commands in the terminal

```shell
$ https://github.com/rbakker96/webserv.git webserv
$ cd webserv

-> Using Makefile
$ make
$ ./webserv ./configuration_files/valid/...         (test working config files different usescases)
$ ./webserv ./configuration_files/invalid/...         (test error handling of different usescases)
```

---

### Used resources
> Most noteworthy resources used during this project

- <a href="https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa" target="_blank">Minimum working example</a>
- <a href="https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers" target="_blank">HTTP Headers</a>
- <a href="http://beej.us/guide/bgnet/html/" target="_blank">Network programming</a>
- <a href="https://en.wikipedia.org/wiki/Common_Gateway_Interface" target="_blank">Common Gateway Interface</a>
