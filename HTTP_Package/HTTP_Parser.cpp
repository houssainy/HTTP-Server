#include "HTTP_Parser.h"


HTTP_Parser::HTTP_Parser()
{
    //ctor
}

void HTTP_Parser::parse_msg(unordered_map<string, char *> *values,char * msg) {
    Dynamic_array *token = new Dynamic_array();

    bool first_line = false;
    int i = 0;
    pair <string, char *> temp_pair;
    for(int i = 0; msg[i] != '\0'; i++) {
        if(!first_line) { // first line
            int token_number = 0 ;
            first_line = true;
            bool request , response ;
            for(; msg[i] != '\r'; i++) {
                if(msg[i] == ' ') {
                    if (token_number==0){
                        request = is_request(token->get_array());
                        response = is_response(token->get_array());
                        if(request)
                            temp_pair= make_pair(HTTP_Utils::METHOD_TYPE,token->get_array());
                        else if(response)
                            temp_pair= make_pair(HTTP_Utils::HTTP_TYPE,token->get_array());
                        values->insert(temp_pair);
                        token = new Dynamic_array();
                    }else if (token_number==1){
                            if (request){
                                temp_pair= make_pair(HTTP_Utils::FILE_NAME,token->get_array());
                                values->insert(temp_pair);
                                token = new Dynamic_array();
                            }
                    }else if (token_number==2){
                             if (request)
                                temp_pair= make_pair(HTTP_Utils::HTTP_TYPE,token->get_array());
                            else if (response)
                                temp_pair= make_pair(HTTP_Utils::STATUS,token->get_array());
                            values->insert(temp_pair);
                            token = new Dynamic_array();
                    } else {
                        cout<<"invalid number of parameter"<<endl;
                        delete token;
                        return ;
                    }
                    token_number++;
                }
                else
                    token->insert(msg[i]);
            }
            i++;
        } else { // header
            token = new Dynamic_array();
            string key = "";
            bool key_token = false;
            for(; msg[i] != '\r'; i++) {
                if(msg[i] == ':' && !key_token) {
                    key += string(token->get_array());
                    token = new Dynamic_array();
                    key_token = true;
                } else
                    token->insert(msg[i]);
            }
            i++;
            temp_pair = make_pair(key, token->get_array());
            values->insert(temp_pair);
        }
    }

    delete token;
}

bool HTTP_Parser::is_request(char *method_type)
{
    if (is_equal(method_type, HTTP_Utils::GET)||is_equal(method_type, HTTP_Utils::POST))
        return true;
    return false ;
}

bool HTTP_Parser::is_response(char *HTTP_type)
{
    if (is_equal(HTTP_type, HTTP_Utils::HTTP1)||is_equal(HTTP_type, HTTP_Utils::HTTP2))
        return true;
    return false;
}

bool HTTP_Parser::is_equal(char* array, string s) {
    for(int i = 0; i < s.size() && array[i] != '\0'; i++) {
        if(array[i] != s[i])
            return false;
    }
    return true;
}
HTTP_Parser::~HTTP_Parser()
{
    //dtor
}
