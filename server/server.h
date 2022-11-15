/*
 * @Author: zy 953725892@qq.com
 * @Date: 2022-11-15 23:26:16
 * @LastEditors: zy 953725892@qq.com
 * @LastEditTime: 2022-11-15 23:41:24
 * @FilePath: /lab3/server/server.h
 * @Description: server相关的数据结构和操作
 * 
 * Copyright (c) 2022 by zy 953725892@qq.com, All Rights Reserved. 
 */
#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>

typedef struct server {
    char* ip;
    char* port;
    char* dir_root;
} server;

// 初始化server
void init_server(server* s, char* ip, char* port, char* dir_root);

// 获取server的socket地址
struct sockaddr_in get_server_addr(server* s);

// 对客户端的请求进行处理
void do_server();

#endif 