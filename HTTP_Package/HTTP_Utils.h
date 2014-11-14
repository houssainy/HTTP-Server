#ifndef HTTP_UTILS_H
#define HTTP_UTILS_H

#include <string>

using namespace std;
class HTTP_Utils
{
    public:
        HTTP_Utils();

        const static string HTTP1;
        const static string HTTP2;
        const static string IMAGE;
        const static string TXT;
        const static string OK;
        const static string NOT_FOUND;
        const static string METHOD_TYPE;
        const static string FILE_NAME;
        const static string HTTP_TYPE;
        const static string STATUS;
        const static string DATA;
        const static string GET;
        const static string POST;
        const static string DATE;
        const static string CONTENT_TYPE;
        const static string CONTENT_LENGTH;
        const static string HOST_NAME;
        const static string ACCEPTED_FILES;
        const static string ACCEPTED_LANGUAGE;
        const static string USER_AGENT;



        virtual ~HTTP_Utils();
    protected:
    private:
};

#endif // HTTP_UTILS_H
