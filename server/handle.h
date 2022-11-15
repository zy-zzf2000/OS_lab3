/*
 * @Author: zy 953725892@qq.com
 * @Date: 2022-11-16 00:44:54
 * @LastEditors: zy 953725892@qq.com
 * @LastEditTime: 2022-11-16 02:23:29
 * @FilePath: /lab3/server/handle.h
 * @Description: 服务器业务处理
 * 
 * Copyright (c) 2022 by zy 953725892@qq.com, All Rights Reserved. 
 */
#ifndef _HANDLE_H
#define _HANDLE_H

#define MAXLINE 1024
#define MAXBUF 4096

#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>

typedef struct handle_arg{
    int client_fd;
    char* base_dir;
}handle_arg;

//根据客户端传输的文件名获取到server上的对应文件
void handle_request(void *arg);


#endif