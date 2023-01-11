/*
 * @Author: zy 953725892@qq.com
 * @Date: 2022-11-16 01:52:47
 * @LastEditors: zy 953725892@qq.com
 * @LastEditTime: 2023-01-11 14:07:02
 * @FilePath: /lab3/server/handle.c
 * @Description: handle.h函数实现
 * 
 * Copyright (c) 2022 by zy 953725892@qq.com, All Rights Reserved. 
 */
#include "handle.h"

extern int show_hide;
extern int show_subdir;

void handle_show(char* base_addr,int client_fd,int show_hide,int show_subdir){
    char buf[MAXBUF];
    strcpy(buf,"file lists:\n");
    send(client_fd,buf,strlen(buf),0);
    //遍历base_addr目录下的所有文件
    DIR *dir = opendir(base_addr);
    struct dirent *ent;
    while((ent=readdir(dir))!=NULL){
        //如果是隐藏文件，且不显示隐藏文件，则跳过
        if(ent->d_name[0]=='.' && !show_hide){
            continue;
        }
        //如果是目录，且不显示子目录，则跳过
        if(ent->d_type==DT_DIR && !show_subdir){
            continue;
        }
        //首先将buf清空
        memset(buf,0,MAXBUF);
        //将文件名拼接到buf中
        memmove(buf + 4 , buf, strlen(buf) + 1);
        memcpy(buf, "    ", 4);
        strcat(buf,ent->d_name);
        strcat(buf,"\n");
        //将buf中的内容发送给客户端
        send(client_fd,buf,strlen(buf),0);
    }
    shutdown(client_fd,SHUT_RDWR);
}

void handle_get(int client_fd,char* filename){
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
        if(n!=send(client_fd,file_buf,n,0)){
            printf("发送文件%s失败",filename);
            return;
        }
    }

    //发送完成之后，需要关闭文件
    fclose(fp);
    //关闭连接
    shutdown(client_fd,SHUT_RDWR);
}

//根据客户端传输的文件名获取到server上的对应文件
void handle_request(void *arg){
    handle_arg *args = (handle_arg*)arg;
    //不断从client中读取指令
    while(1){
        char buf[MAXLINE];
        int n = recv(args->client_fd,buf,MAXLINE,0);
        if(n<0){
            perror("recv error");
            return;
        }else if(n==0){
            printf("client close\n");
            return;
        }
        //TODO:根据指令处理客户端请求
        if (strncmp(buf, "show", 4) == 0) {
            handle_show(args->base_dir,args->client_fd,show_hide,show_subdir);
            memset(buf, 0, MAXLINE);
        } else if (strncmp(buf, "get", 3) == 0) {
            //打开客户端请求文件
            char* filename = (char*)malloc(sizeof(char)*(strlen(args->base_dir)+strlen(buf+4)));
            sprintf(filename,"%s/%s",args->base_dir,buf+4);
            handle_get(args->client_fd, filename);
            printf("处理完成对于文件：%s的请求", buf + 4);
            return;
        } else {
            printf("unknown command: %s\n", buf);
            return;
        }
    }
}

