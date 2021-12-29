//
// Created by 裴沛东 on 2021/12/27.
//


#include "util.h"
void errif(bool condition,const char* message) {
    if(condition) {
        perror(message);
        exit(EXIT_FAILURE);
    }
 }
