#include "HTTP_Generator.h"


HTTP_Generator::HTTP_Generator(string name )
{
    host_name = name ;
}

string HTTP_Generator::generate_get_request(string file_name ,string HTTP_type  )
{
    string msg (HTTP_Utils::GET + " " + file_name + " " + HTTP_type + "\r\n");
    msg += HTTP_Utils::HOST_NAME+":" + get_host_name() + "\r\n";
    msg += HTTP_Utils::ACCEPTED_FILES + ":" + get_accepted_types() + "\r\n";
    msg += HTTP_Utils::ACCEPTED_LANGUAGE +":" + get_accepted_language() + "\r\n";
    msg += HTTP_Utils::USER_AGENT +":" + get_user_agent() + "\r\n\r\n";
    return msg ;
}

string HTTP_Generator::generate_post_request(string file_name ,string HTTP_type , string file_type , int data_lenght)
{
    string msg (HTTP_Utils::POST + " " +file_name + " " + HTTP_type + "\r\n");
    msg += HTTP_Utils::HOST_NAME + ":" + get_host_name() + "\r\n";
    msg += HTTP_Utils::ACCEPTED_FILES + ":" + get_accepted_types() + "\r\n";
    msg += HTTP_Utils::ACCEPTED_LANGUAGE + ":" + get_accepted_language() + "\r\n";
    msg += HTTP_Utils::USER_AGENT + ":" + get_user_agent() + "\r\n";
    msg += HTTP_Utils::CONTENT_TYPE + ":" + file_type + "\r\n";
    if (data_lenght>0)
        msg += HTTP_Utils::CONTENT_LENGTH + ":"+ to_string(data_lenght) + "\r\n";
    msg += "\r\n";
    return msg ;
}


string HTTP_Generator::generate_get_response(string HTTP_type ,string state ,string file_type , int data_lenght)
{
    string msg (HTTP_type + " " + state + "\r\n");
    msg += HTTP_Utils::DATE + ":" + get_data() + "\r\n";
    msg += HTTP_Utils::USER_AGENT + ":" + get_user_agent() + "\r\n";
    msg += HTTP_Utils::CONTENT_TYPE + ":" + file_type + "\r\n";
    if (data_lenght>0)
        msg += HTTP_Utils::CONTENT_LENGTH + ":"+ to_string(data_lenght) + "\r\n";
    msg += "\r\n";
    return msg ;
}


string HTTP_Generator::generate_post_response( string HTTP_type ,string state ,string file_type )
{
    string msg (HTTP_type + " " + state + "\r\n");
    msg += HTTP_Utils::DATE + ":"   + get_data() + "\r\n";
    msg += HTTP_Utils::USER_AGENT + ":" + get_user_agent() + "\r\n";
    msg += HTTP_Utils::CONTENT_TYPE + ":" + file_type + "\r\n\r\n";
    return msg ;
}

string HTTP_Generator::get_host_name ()
{
    ///TODO get hostname
    return host_name;
}

string HTTP_Generator::get_accepted_language ()
{
    ///TODO get accepted language from system
    return ("en-us");
}

string HTTP_Generator::get_user_agent()
{
    /// TODO get user agent from system
    return ("Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)");
}

string HTTP_Generator::get_accepted_types()
{
    ///TODO get accepted files from system
    return ("*/*");

}

string HTTP_Generator::get_data()
{
    ///TODO get data from system
    return ("Sun, 16 Nov 2014 12:10:12 GMT");

}

HTTP_Generator::~HTTP_Generator()
{
    //dtor
}
