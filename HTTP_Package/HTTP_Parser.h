#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <sstream>
#include "HTTP_Utils.h"


using namespace std;

class HTTP_Parser
{
    public:
        HTTP_Parser();
        unordered_map <string , string > parse_msg(string msg);
        virtual ~HTTP_Parser();
    protected:
    private:
        void parse_first_line(unordered_map<string, string> &map,string line);
        void parse_line(unordered_map<string, string> &map,string line);
        bool is_request(string method_type);
        bool is_response(string HTTP_type);
};

#endif // HTTP_PARSER_H
