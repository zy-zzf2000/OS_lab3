/*
 * @Author: zy 953725892@qq.com
 * @Date: 2022-11-16 00:44:54
 * @LastEditors: zy 953725892@qq.com
 * @LastEditTime: 2023-01-11 20:53:37
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
#include <dirent.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct handle_arg{
    int client_fd;
    char* base_dir;
}handle_arg;

//处理show指令
void handle_show(char* base_addr,int client_fd,int show_hide,int show_subdir);

//处理get指令
void handle_get(int client_fd,char* filename);

//根据客户端传输的文件名获取到server上的对应文件
void handle_request(void *arg);

//向指定的客户端发送文件(夹)
void send_file(int client_fd, char* filename);


#endif