#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <sstream>
#include "HTTP_Utils.h"
#include "../dynamic_array_package/Dynamic_array.h"


using namespace std;

class HTTP_Parser
{
    public:
        HTTP_Parser();
        void parse_msg(unordered_map<string, char *> *values,char* msg);
        virtual ~HTTP_Parser();
    protected:
    private:
        bool is_request(char * method_type);
        bool is_response(char *HTTP_type);
        bool is_equal(char* array, string s);
};

#endif // HTTP_PARSER_H
