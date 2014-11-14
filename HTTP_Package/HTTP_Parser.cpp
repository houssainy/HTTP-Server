#include "HTTP_Parser.h"


HTTP_Parser::HTTP_Parser()
{
    //ctor
}

unordered_map <string, string> HTTP_Parser::parse_msg(string msg)
{
    vector <string> temp;
    unordered_map <string, string> values;
    pair <string, string> data_pair (HTTP_Utils::DATA ,"");
    values.insert(data_pair);
    istringstream ss (msg);
    string token;
    int line = 0;
    bool data_flag = false ;
    while (getline(ss, token, '\n'))
    {
        if (token.size()==0)
            data_flag = true;
        if (line == 0)
        {
            parse_first_line(values, token);
            line++;
        }
        else if (!data_flag)
        {
            parse_line (values, token);
            line++;
        }
        else if (token.size()!=0)
            values.at(HTTP_Utils::DATA) +=token+"\n";


    }
    for (auto& x: values) {
        cout << x.first << ": " << x.second << endl;
    }
    return values;
}
void HTTP_Parser::parse_first_line(unordered_map<string, string> &map, string line)
{
    istringstream temp_ss (line);
    string sub_token ;
    int token_number = 0 ;
    bool request , response ;
    while(getline(temp_ss,sub_token, ' '))
    {
        pair <string, string> temp_pair;
        if (token_number ==0 )
        {
            request = is_request(sub_token);
            response = is_response(sub_token);
            if(request)
                temp_pair= make_pair(HTTP_Utils::METHOD_TYPE,sub_token);
            else if(response)
                temp_pair= make_pair(HTTP_Utils::HTTP_TYPE,sub_token);
            map.insert(temp_pair);
        }
        else if (token_number ==1 )
        {
            if (request)
                temp_pair= make_pair(HTTP_Utils::FILE_NAME,sub_token);
            else if (response)
                temp_pair= make_pair(HTTP_Utils::STATUS,sub_token);
            map.insert(temp_pair);
        }
        else
        {
            if (request)
            {

                temp_pair= make_pair(HTTP_Utils::HTTP_TYPE,sub_token);
                map.insert(temp_pair);
            }
            else if (response)
                map.at(HTTP_Utils::STATUS)+=" "+sub_token;
        }
        token_number ++;
    }

}

void HTTP_Parser::parse_line(unordered_map<string, string> &map,string line)
{
    istringstream temp_ss (line);
    string sub_token , key;
    int token_number =0 ;
    while(getline(temp_ss,sub_token, ':'))
    {
        if (token_number== 0)
            key = sub_token;
        else if (token_number==1)
        {
            pair <string, string> temp_pair (key,sub_token);
            map.insert(temp_pair);

        }
        else
        {
            map.at(key) += ":"+sub_token;
        }
        token_number ++;
    }
}

bool HTTP_Parser::is_request(string method_type)
{
    if (method_type == HTTP_Utils::GET ||method_type == HTTP_Utils::POST )
        return true;
    return false ;
}

bool HTTP_Parser::is_response(string HTTP_type)
{
    if (HTTP_type==HTTP_Utils::HTTP1||HTTP_type==HTTP_Utils::HTTP2)
        return true;
    return false;
}

HTTP_Parser::~HTTP_Parser()
{
    //dtor
}
