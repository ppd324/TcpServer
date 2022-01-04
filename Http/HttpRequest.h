//
// Created by 裴沛东 on 2021/12/31.
//

#ifndef TCPSERVER_HTTPREQUEST_H
#define TCPSERVER_HTTPREQUEST_H
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <regex>
#include "../Sql/SqlConnRAII.h"
#include "../Buffer.h"
#include "../util.h"


class HttpRequest {
public:
    enum PARSE_STATE{
        REQUEST_LINE_,
        HEADERS_,
        BODY_,
        FINISH_,
    };
    enum HTTP_CODE {
        NO_REQUEST = 0,
        GET_REQUEST,
        BAD_REQUEST,
        NO_RESOURSE,
        FORBIDDENT_REQUEST,
        FILE_REQUEST,
        INTERNAL_ERROR,
        CLOSED_CONNECTION,
    };
    HttpRequest();
    void init();
    ~HttpRequest() = default;
    bool parse(Buffer &buffer);

    std::string& path();
    std::string& method();
    std::string& version();
    std::string& body();
    std::string GetPost(const std::string &key) const;
    std::string GetPost(const char* key) const;
    bool IsKeepAlive() const;
    void printHead();
    bool UserVerify(const string &name, const string &pwd, bool isLogin);
private:
    bool ParseRequestLine_(const std::string& line);
    void ParseHeader_(const std::string& line);
    void ParseBody_(const std::string& line);

    void ParsePath_();
    void ParsePost_();
    void ParseFromUrlencoded_();
    PARSE_STATE parse_state_;
    std::string method_,path_,version_,body_;
    std::unordered_map<std::string,std::string> header_;
    std::unordered_map<std::string,std::string> post_;

    static const std::unordered_set<std::string> DEFAULT_HTML;
    static const std::unordered_map<std::string, int> DEFAULT_HTML_TAG;
    static int ConverHex(char ch);


};


#endif //TCPSERVER_HTTPREQUEST_H
