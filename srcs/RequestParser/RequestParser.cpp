#include "RequestParser.hpp"

RequestParser::RequestParser(std::string request)
{
    this->full_request = request.c_str();
    this->parse(request);
}

RequestParser::~RequestParser(){}

std::string RequestParser::get_next(std::string &str, const std::string delimiter){
    std::string token;
    size_t      i;

    // std::cout << "str = " << str << std::endl;
    if ((i = str.find(delimiter)) != std::string::npos){
        token = str.substr(0, i);
        str.erase(0, i + delimiter.length());
        return token;
    }else{
        throw "No next";
    }
}

void    RequestParser::parse(std::string request){
    std::string delimiter("\r\n"), initial_request_line_delimiter(" "), headers_delimiter(":");
    std::string token, key, last_key;

    // std::cout << "---------------------------------" << std::endl;
    // std::cout << request << std::endl;
    // std::cout << "---------------------------------" << std::endl;
    try{
        token = this->get_next(request, delimiter);
        // std::cout << "token = " << token << std::endl;
        this->command = this->get_next(token, initial_request_line_delimiter);
        // std::cout << "command = " << this->command << std::endl;
        this->path = this->get_next(token, initial_request_line_delimiter);
        this->path.erase(0, 1);
        // std::cout << "path = " << this->path << std::endl;
        // std::cout << "token = " << token << std::endl;
        token.erase(0, 5);
        // std::cout << "tmp_HTTP_version = " << token << std::endl;
        this->HTTP_version = std::stod(token);
        token = this->get_next(request, delimiter);
        // std::cout << "token = " << token << std::endl;
        while (token != ""){
            try{
                key = this->get_next(token, headers_delimiter);
                while(token[0] == ' ' || token[0] == '\t'){
                    token.erase(0, 1); 
                }
                this->headers.insert(std::pair<std::string, std::string>(key, token));
                last_key = key;
            }
            catch(const char *str){
                if (this->headers.size() == 0){
                    throw "Bad headers";
                }
                while(token[0] == ' ' || token[0] == '\t'){
                    token.erase(0, 1); 
                }
                this->headers[last_key] += token;
            }
            token = this->get_next(request, delimiter);
            // std::cout << "token = " << token << std::endl;
        }
        this->body = request;
    }
    catch (const char *str){
        std::cout << str << std::endl;
    }
}
