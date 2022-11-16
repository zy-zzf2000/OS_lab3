/*
 * @Author: zy 953725892@qq.com
 * @Date: 2022-11-15 23:26:16
 * @LastEditors: zy 953725892@qq.com
 * @LastEditTime: 2022-11-16 17:45:10
 * @FilePath: /lab3/server/server.h
 * @Description: server相关的数据结构和操作
 * 
 * Copyright (c) 2022 by zy 953725892@qq.com, All Rights Reserved. 
 */
#ifndef _SERVER_H
#define _SERVER_H

#include <netinet/in.h>
#include "handle.h"
#include <pthread.h>

typedef unsigned short ushort;

typedef struct server {
    char* ip;
    ushort port;
    char* dir_root;
    int listen_fd;
} server;

// 初始化server
void init_server(server* s, char* ip, ushort port, char* dir_root);

// 对客户端的请求进行处理
void do_server(server* s);

#endif 