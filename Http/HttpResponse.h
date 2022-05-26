//
// Created by 裴沛东 on 2021/12/31.
//

#ifndef TCPSERVER_HTTPRESPONSE_H
#define TCPSERVER_HTTPRESPONSE_H
#include <unordered_map>
#include <memory>

#include "../Buffer.h"
#include "../util.h"

class HttpResponse {
public:
    HttpResponse();
    ~HttpResponse();

    void Init(const std::string& srcDir, std::string& path, bool isKeepAlive = false, int code = -1);
    void makeResponse(std::shared_ptr<Buffer> &buffer);
    void unmapFile();
    char* File();
    size_t FileLen() const;
    void errorContent(std::shared_ptr<Buffer> &buffer, std::string message) const;
    int Code() const { return code_; }

private:
    void addStateLine_(std::shared_ptr<Buffer> &buffer);
    void addHeader_(std::shared_ptr<Buffer> &buffer);
    void addContent_(std::shared_ptr<Buffer> &buffer);

    void errorHtml_();
    std::string getFileType_();

    int code_;
    bool isKeepAlive_;

    std::string path_;
    std::string srcDir_;

    char* mmFile_;
    struct stat mmFileStat_;

    static const std::unordered_map<std::string, std::string> SUFFIX_TYPE;
    static const std::unordered_map<int, std::string> CODE_STATUS;
    static const std::unordered_map<int, std::string> CODE_PATH;
};


#endif //TCPSERVER_HTTPRESPONSE_H
