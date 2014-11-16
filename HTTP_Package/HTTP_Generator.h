#ifndef HTTP_GENERATOR_H
#define HTTP_GENERATOR_H
#include <string>
#include <string.h>
#include <iostream>
#include "../dynamic_array_package/Dynamic_array.h"
#include "HTTP_Utils.h"

using namespace std ;
class HTTP_Generator
{
    public:


        HTTP_Generator(string name);

        string generate_get_request (string file_name ,string HTTP_type );
        string generate_post_request(string file_name ,string HTTP_type , string file_type , int data_lenght);

        string generate_get_response(string HTTP_type ,string state ,string file_type , int data_lenght);
        string generate_post_response(string HTTP_type ,string state ,string file_type );

        virtual ~HTTP_Generator();


    private:
        string host_name;

        string get_host_name ();
        string get_accepted_language ();
        string get_user_agent();
        string get_accepted_types();
        string get_data();
};


#endif // HTTP_GENERATOR_H
