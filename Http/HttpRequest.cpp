//
// Created by 裴沛东 on 2021/12/31.
//

#include <regex>
#include <cassert>
#include "HttpRequest.h"
const std::unordered_set<std::string> HttpRequest::DEFAULT_HTML{
        "/index", "/register", "/login",
        "/welcome", "/video", "/picture", };
const std::unordered_map<std::string, int> HttpRequest::DEFAULT_HTML_TAG {
        {"/register.html", 0}, {"/login.html", 1},  };

HttpRequest::HttpRequest() {
    init();

}

void HttpRequest::ParseBody_(const std::string &line) {
    body_ = line;
    ParsePost_();
    parse_state_ = FINISH_;
    LOG_DEBUG<<"Body:"<<line.c_str()<<" size is"<<line.size();

}

std::string& HttpRequest::path(){
    return path_;
}

bool HttpRequest::parse(Buffer &buffer) {
    if(!buffer.readable()) {
        return false;
    }
    while(buffer.readable() && parse_state_ != FINISH_) {
        std::string line = std::move(buffer.getline());
        switch(parse_state_) {
            case REQUEST_LINE_:
                if(!ParseRequestLine_(line)) {
                    return false;
                }
                ParsePath_();
                break;
            case HEADERS_:
                ParseHeader_(line);
                if(buffer.readableBytes() <= 2) {
                    parse_state_ = FINISH_;
                }
                break;
            case BODY_:
                ParseBody_(line);
                break;
            default:
                break;
        }
    }

    return false;
}

std::string& HttpRequest::method() {
    return method_;
}

std::string& HttpRequest::version() {
    return version_;
}

std::string HttpRequest::GetPost(const std::string &key) const {
    return std::string();
}

std::string HttpRequest::GetPost(const char *key) const {
    return std::string();
}

bool HttpRequest::IsKeepAlive() const {
    if(header_.count("Connection") == 1) {
        return header_.find("Connection")->second == "keep-alive" && version_ == "1.1";
    }
    return false;
}

bool HttpRequest::ParseRequestLine_(const std::string &line) {
    std::regex patten("^([^ ]*) ([^ ]*) HTTP/([^ ]*)$");
    std::smatch subMatch;
    if(std::regex_match(line, subMatch, patten)) {
        method_ = subMatch[1];
        path_ = subMatch[2];
        version_ = subMatch[3];
        parse_state_ = HEADERS_;
        return true;
    }
    LOG_ERROR<<"parse requestLine error";
    return false;


}

void HttpRequest::ParseHeader_(const std::string &line) {
    regex patten("^([^:]*): ?(.*)$");
    smatch subMatch;
    if(regex_match(line, subMatch, patten)) {
        header_[subMatch[1]] = subMatch[2];
    }
    else {
        parse_state_ = BODY_;
    }
}

void HttpRequest::ParsePath_() {
    if(path_ == "/") {
        path_ = "/index.html";
    }else {
        for(auto &it : DEFAULT_HTML) {
            if(it == path_) {
                path_ += ".html";
                break; //end
            }
        }
    }

}

void HttpRequest::ParsePost_() {
    if(method_ == "POST" && header_["Content-Type"] == "application/x-www-form-urlencoded") {
        ParseFromUrlencoded_();
        if(DEFAULT_HTML_TAG.count(path_)) {
            int tag = DEFAULT_HTML_TAG.find(path_)->second;
            LOG_DEBUG<<"tag is "<<tag;
            if(tag == 0 || tag == 1) {
                bool isLogin = (tag == 1);
                if(UserVerify(post_["username"],post_["password"],isLogin)) {
                    path_ = "/welcome.html";
                }else {
                    path_ = "/error.html";
                }
            }
        }
    }

}
bool HttpRequest::UserVerify(const string &name, const string &pwd, bool isLogin) {

}
void HttpRequest::ParseFromUrlencoded_() {
    if(body_.size() == 0) { return; }
    string key, value;
    int num = 0;
    int n = body_.size();
    int i = 0, j = 0;

    for(; i < n; i++) {
        char ch = body_[i];
        switch (ch) {
            case '=':
                key = body_.substr(j, i - j);
                j = i + 1;
                break;
            case '+':
                body_[i] = ' ';
                break;
            case '%':
                num = ConverHex(body_[i + 1]) * 16 + ConverHex(body_[i + 2]);
                body_[i + 2] = num % 10 + '0';
                body_[i + 1] = num / 10 + '0';
                i += 2;
                break;
            case '&':
                value = body_.substr(j, i - j);
                j = i + 1;
                post_[key] = value;
                LOG_DEBUG<<key.c_str()<<"->->"<<value.c_str();
                break;
            default:
                break;
        }
    }
    assert(j <= i);
    if(post_.count(key) == 0 && j < i) {
        value = body_.substr(j, i - j);
        post_[key] = value;
    }
}

int HttpRequest::ConverHex(char ch) {
    if(ch >= 'A' && ch <= 'F') return ch -'A' + 10;
    if(ch >= 'a' && ch <= 'f') return ch -'a' + 10;
    return ch;
}

void HttpRequest::init() {
    method_ = path_ = version_ = body_ = "";
    parse_state_ = REQUEST_LINE_;
    header_.clear();
    post_.clear();

}

void HttpRequest::printHead() {
    for(auto &it : header_) {
        std::cout<<it.first<<"->"<<it.second<<std::endl;
    }
}

std::string & HttpRequest::body() {
    return body_;
}