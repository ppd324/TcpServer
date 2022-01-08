//
// Created by 裴沛东 on 2022/1/3.
//

#include <cassert>
#include "SqlConnPool.h"

SqlConnPool *SqlConnPool::Instance() {
    static SqlConnPool connPool;
    return &connPool;
}

MYSQL *SqlConnPool::getConn() {
    MYSQL* conn = nullptr;
    if(_connQue.empty()) {
        LOG_WARN<<"sqlconnpool is busy";
        return nullptr;
    }
    sem_wait(&_sem);
    {
        std::lock_guard<std::mutex> lock(_mutex);
        conn = _connQue.front();
        _connQue.pop();
    }
    return conn;
}

void SqlConnPool::freeConn(MYSQL *conn) {
    assert(conn);
    std::lock_guard<std::mutex> lock(_mutex);
    _connQue.push(conn);
    sem_post(&_sem);

}

void
SqlConnPool::Init(const char *host, int port, const char *user, const char *pwd, const char *dbName, int connSize) {
    assert(connSize > 0);
    for(int i=0;i<connSize;++i) {
        MYSQL *conn = nullptr;
        conn = mysql_init(conn);
        if(!conn) {
            LOG_ERROR<<"mysql init error";
        }
        conn = mysql_real_connect(conn,host,user,pwd,dbName,port,nullptr,0);
        if(!conn) {
            LOG_ERROR<<"mysql connect error";
        }else {
            LOG_DEBUG<<"mysql connect success";
        }
        _connQue.push(conn);
    }
    _MAX_CONN_SIZE = connSize;
    sem_init(&_sem,0,_MAX_CONN_SIZE);

}

int SqlConnPool::getFreeConnCount() {
    std::lock_guard<std::mutex> lock(_mutex);
    return _connQue.size();

}

void SqlConnPool::closePool() {
    std::lock_guard<std::mutex> lock(_mutex);
    while(!_connQue.empty()) {
        auto it = _connQue.front();
        _connQue.pop();
        mysql_close(it);
    }
    mysql_library_end();

}

SqlConnPool::SqlConnPool():_USE_COUNT_(0),_FREE_COUNT_(0) {
    Init("127.0.0.1",3306,"root","123456","serverUser",3);
}

SqlConnPool::~SqlConnPool() {
    closePool();

}
