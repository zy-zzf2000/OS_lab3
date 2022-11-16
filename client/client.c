/*
 * @Author: zy 953725892@qq.com
 * @Date: 2022-11-16 16:14:17
 * @LastEditors: zy 953725892@qq.com
 * @LastEditTime: 2022-11-16 16:54:30
 * @FilePath: /lab3/client/client.c
 * @Description: client.h函数实现
 * 
 * Copyright (c) 2022 by zy 953725892@qq.com, All Rights Reserved. 
 */
#include "client.h"

//初始化客户端，得到服务器的socket
void init_client(client* c, char* server_ip, uint server_port, char* request_file,char* save_name){
    c->request_file = request_file;
    c->save_name = save_name==NULL?request_file:save_name;
    //创建套接字
    int socket_fd = socket(AF_INET,SOCK_STREAM,0);
    if (socket_fd < 0){
        perror("socket 创建错误!\n");
        exit(0);
    }

    //连接服务器
    struct sockaddr_in server_addr;
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htonl(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    if(connect(socket_fd,(struct sockaddr*)&server_addr,sizeof(server_addr)) < 0){
        perror("connect 错误!\n");
        exit(0);
    }

    c->fd = socket_fd;
}