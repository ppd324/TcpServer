//
// Created by 裴沛东 on 2022/1/3.
//

#ifndef TCPSERVER_SQLCONNRAII_H
#define TCPSERVER_SQLCONNRAII_H
#include <mysql/mysql.h>
#include <cassert>
#include "SqlConnPool.h"

class SqlConnRAII {
public:
    SqlConnRAII(MYSQL** sql,SqlConnPool* pool) {
        assert(pool);
        *sql = pool->getConn();
        _conn = *sql;
        _pool = pool;
    }

    ~SqlConnRAII() {
        if(_conn) {
            _pool->freeConn(_conn);
        }
    }

private:
    MYSQL* _conn;
    SqlConnPool* _pool;

};


#endif //TCPSERVER_SQLCONNRAII_H
