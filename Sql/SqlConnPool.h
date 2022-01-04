//
// Created by 裴沛东 on 2022/1/3.
//

#ifndef TCPSERVER_SQLCONNPOOL_H
#define TCPSERVER_SQLCONNPOOL_H
#include <mysql/mysql.h>
#include <queue>
#include <mutex>
#include <thread>
#include <semaphore.h>
#include "../util.h"

class SqlConnPool {
public:
    static SqlConnPool *Instance();

    MYSQL* getConn();
    void freeConn(MYSQL* conn);
    void Init(const char* host, int port,
              const char* user,const char* pwd,
              const char* dbName, int connSize);
    int getFreeConnCount();
    void closePool();
private:
    SqlConnPool();
    ~SqlConnPool();

    int _MAX_CONN_SIZE;
    int _USE_COUNT_;
    int _FREE_COUNT_;

    std::queue<MYSQL*> _connQue;
    std::mutex _mutex;
    sem_t _sem;

};


#endif //TCPSERVER_SQLCONNPOOL_H
