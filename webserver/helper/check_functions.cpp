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

