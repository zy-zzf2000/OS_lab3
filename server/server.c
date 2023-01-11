/*
 * @Author: zy 953725892@qq.com
 * @Date: 2022-11-15 23:43:47
 * @LastEditors: zy 953725892@qq.com
 * @LastEditTime: 2023-01-11 12:11:23
 * @FilePath: /lab3/server/server.c
 * @Description: server.h函数实现
 * 
 * Copyright (c) 2022 by zy 953725892@qq.com, All Rights Reserved. 
 */
#include "server.h"

// 初始化server
void init_server(server* s, char* ip, ushort port, char* dir_root){

    s->ip = ip;
    s->port = port;
    s->dir_root = dir_root;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    s->listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(s->listen_fd < 0){
        perror("listen socket 创建错误");
        exit(0);
    }

    if(bind(s->listen_fd,(struct sockaddr*)&addr,sizeof(addr)) < 0){
        perror("bind 错误");
        exit(0);
    }
    printf("server listening at %s:%d\n",ip, port);
}

void show_dir(server* s,int showHidden, int showSubDir){
    DIR *dir;
    struct dirent *ent;
    printf("file list:\n");
    if((dir = opendir(s->dir_root))!=NULL){
        while((ent = readdir(dir))!=NULL){
            if(ent->d_name[0] == '.' && !showHidden){
                continue;
            }
            if(ent->d_type == DT_DIR && !showSubDir){
                continue;
            }
            printf("    %s\n",ent->d_name);
        }
    }
}

// 对客户端的请求进行处理
void do_server(server* s){
    //监听套接字
    if(listen(s->listen_fd, 10) < 0){
        perror("listen 错误");
        exit(0);
    }

    while(1){
        //等待accept
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        int client_fd = accept(s->listen_fd, (struct sockaddr*)&client_addr, &len);
        if(client_fd < 0){
            perror("accept 错误");
            exit(0);
        }else{
            //开新线程处理client请求
            handle_arg* arg = (handle_arg*)malloc(sizeof(handle_arg));
            arg->client_fd = client_fd;
            arg->base_dir = s->dir_root;
            pthread_t tid;
            pthread_create(&tid, NULL, (void*)handle_request, (void*)arg);
        }
    }
}