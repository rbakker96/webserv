#include "helper.hpp"

int     check_server_block(std::vector<std::string> server_block) {
    int         open_bracket = 0;
    int         closing_bracket = 0;
    std::string str;

    for (std::vector<std::string>::iterator it = server_block.begin(); it != server_block.end(); it++) {
        str = *it;
        if (str.find('{') != std::string::npos)
            open_bracket++;
        if (str.find('}') != std::string::npos)
            closing_bracket++;
    }
    if (open_bracket == closing_bracket && open_bracket != 0)
        return 1;
    return 0;
}

int    location_block_size(vector_iterator it, vector_iterator end) {
    for (int i = 0; it != end; it++) {
        std::string str = *it;

        if (str.find("}") != std::string::npos)
            return i;
        i++;
    }
    return 0;
}

int     validate_request(const std::string& request) {
    int header_size;
    int pos;

    if ((header_size = (int)request.find("\r\n\r\n")) != -1) {
        if ((pos = (int)request.find("Content-Length:")) == -1)
            return valid_;
        int content_length = ft_atoi(request.c_str() + (pos + 16));
        std::string body = request.substr(header_size + 4);
        if ((int)body.length() == content_length)
            return valid_;
    }
    return invalid_;
}

