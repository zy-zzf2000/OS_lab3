/*
 * @Author: zy 953725892@qq.com
 * @Date: 2022-11-16 01:52:47
 * @LastEditors: zy 953725892@qq.com
 * @LastEditTime: 2022-11-16 01:53:15
 * @FilePath: /lab3/server/handle.c
 * @Description: handle.h函数实现
 * 
 * Copyright (c) 2022 by zy 953725892@qq.com, All Rights Reserved. 
 */
#include "handle.h"

//根据客户端传输的文件名获取到server上的对应文件
void handle_request(void *arg){
    handle_arg *args = (handle_arg*)arg;
    //首先需要从client中读取文件名
    char buf[MAXLINE];
    int n = recv(args->client_fd,buf,MAXLINE,0);
    if(n<0){
        perror("recv error");
        return;
    }

    //打开客户端请求文件
    char* filename = (char*)malloc(sizeof(char)*(strlen(args->base_dir)+strlen(buf)));
    sprintf(filename,"%s/%s",args->base_dir,buf);
    FILE *fp = fopen(filename,"r");
    if(fp==NULL){
        printf("文件%s不存在",filename);
        return;
    }

    //读取文件内容到buf中，准备发送给客户端
    //注意文件可能很大，需要循环读取fp stream，直到EOF为止（可用feof判断）
    char file_buf[MAXBUF];
    while(!feof(fp)){
        //首先将file_buf清空
        memset(file_buf,0,MAXBUF);
        //然后将fp中的内容读取到file_buf中
        int n = fread(file_buf,1,MAXBUF,fp);
        if(n<0){
            printf("读取文件%s失败",filename);
            return;
        }
        //将file_buf中的内容发送给客户端
        if(n!=send(args->client_fd,file_buf,n,0)){
            printf("发送文件%s失败",filename);
            return;
        }
    }

    //发送完成之后，需要关闭文件
    fclose(fp);
    //关闭连接
    shutdown(args->client_fd,SHUT_RDWR);
}

