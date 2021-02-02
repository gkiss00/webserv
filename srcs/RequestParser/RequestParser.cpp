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
    // std::cout << "find\n";
    if ((i = str.find(delimiter)) != std::string::npos){
        // std::cout << "end find\n";
        token = str.substr(0, i);
        str.erase(0, i + delimiter.length());
        return token;
    }else{
        throw request_exception("No next.", 400);
    }
}

void    RequestParser::parse(std::string request){
    std::string delimiter("\r\n"), initial_request_line_delimiter(" "), headers_delimiter(":");
    std::string token, key, last_key;
    bool        just_1_line(false);
    bool        is_content_lenght_exists(false);
    bool        is_chuncked(false);

    std::cout << "--------------- Raw Input ------------------" << std::endl;
    std::cout << request.size() << std::endl;

    request = request.substr(0, REQUEST_MAX_SIZE);

    for (int i = 0; request.substr(0, 1000)[i] != '\0'; ++i){
        if (isspace(request[i])){
            std::cout << "\\";
        }else{
            std::cout << request[i];
        }
    }
    request.substr(0, 1000);
    std::cout << "---------------------------------" << std::endl;

    try{
        token = this->get_next(request, delimiter);
    }catch(request_exception &e){
        token = request;
        just_1_line = true;
    }
    this->command = this->get_next(token, initial_request_line_delimiter);
    try{
        this->path = this->get_next(token, initial_request_line_delimiter);
    }catch(request_exception &e){
        this->path = token;
        if (this->path.size() > URL_MAX_SIZE){
            throw request_exception("Url too long.", 414);
        }else{
            throw request_exception("No next.", 400);
        }
    }
    if (this->path.size() > URL_MAX_SIZE){
        throw request_exception("Url too long.", 414);
    }
    this->path.erase(0, 1);
    token.erase(0, 5);
    try{
        this->HTTP_version = std::stod(token);
    }catch(std::exception &e){
        throw request_exception(e.what(), 400);
    }
    if (this->HTTP_version != 1.1){
        throw request_exception("Bad http version.", 400);
    }
    if (just_1_line == false){
        token = this->get_next(request, delimiter);
        while (token != ""){
            try{
                key = this->get_next(token, headers_delimiter);
                if (isspace(key.back())){
                    throw request_exception("No space between header-name and colon.", 400);
                }
                if (key.size() == 0){
                    throw request_exception("Empty header-name.", 400);
                }
                while (isspace(token[0])){
                    token.erase(0, 1); 
                }
                if (key == "Transfer-Encoding" && token == "chunked"){
                    is_chuncked = true;
                }
                if (key == "Content-Length"){
                    if (is_content_lenght_exists == false){
                        is_content_lenght_exists = true;
                    }else{
                        throw request_exception("Content-lenght already set", 400);
                    }
                    int lenght;
                    try{
                        lenght = std::stoi(token);
                    }catch(std::exception &e){
                        throw request_exception("Bad content-lenght value.", 400);
                    }
                    if (lenght < 0 || lenght > CONTENT_LENGHT_MAX){
                        throw request_exception("Bad content-lenght value.", 400);
                    }
                }
                this->headers.insert(std::pair<std::string, std::string>(key, token));
                last_key = key;
            }
            catch(request_exception &e){
                if (e.what() != std::string("No next.")){
                    throw e;
                }
                if (this->headers.size() == 0){
                    throw request_exception("Bad headers.", 400);
                }
                while (isspace(token[0])){
                    token.erase(0, 1); 
                }
                this->headers[last_key] += token;
            }
            token = this->get_next(request, delimiter);
        }
        this->body = request;
        if (is_chuncked){
            std::string size_hex;
            std::string new_body;
            int size;
            size_t pos;
            size_t i = 0;

            while (i < this->body.size()){
                
                std::cout << "i = " << i << std::endl;
                pos = this->body.find("\r\n", i);
                std::cout << "pos = " << pos << std::endl;

                size_hex = this->body.substr(i, pos - i);

                std::cout << "size_hex = '" << size_hex.size() << " et " << size_hex.substr(0, 100) << "'" << std::endl;
                size = (int)std::stol(size_hex, nullptr, 16);
                if (size == 0){
                    break ;
                }
                i += size_hex.size() + 2;;
                std::cout << "size = " << size << std::endl;
                new_body.append(this->body, i, size);
                i += size + 2;
            }
            this->body = new_body;
        }
    }else{
        this->body = "";
    }
}
