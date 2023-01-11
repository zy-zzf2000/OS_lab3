/*
 * @Author: zy 953725892@qq.com
 * @Date: 2022-11-16 14:37:33
 * @LastEditors: zy 953725892@qq.com
 * @LastEditTime: 2023-01-12 00:09:31
 * @FilePath: /lab3/client/client.h
 * @Description: 
 * 
 * Copyright (c) 2022 by zy 953725892@qq.com, All Rights Reserved. 
 */
#ifndef _CLIENT_H
#define _CLIENT_H

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned short ushort;

typedef struct client {
    char* request_file;
    char* save_name;
    int   file_size;
    int fd;
} client;

//初始化客户端，得到服务器的socket
void init_client(client* c, char* server_ip, ushort server_port, char* request_file,char* save_name);

#endif