/*
 * @Author: zy 953725892@qq.com
 * @Date: 2022-11-16 16:28:01
 * @LastEditors: zy 953725892@qq.com
 * @LastEditTime: 2022-11-16 23:11:31
 * @FilePath: /lab3/client/request.c
 * @Description: 
 * 
 * Copyright (c) 2022 by zy 953725892@qq.com, All Rights Reserved. 
 */
#include "request.h"

void request_show(client *c){
    char buf[MAX_BUFF_SIZE];
    //首先向客户端发送show指令
    if(send(c->fd,"show",4,0)<0){
        printf("发送show指令失败");
        return;
    }
    //从服务器接收数据
    while(1){
        memset(buf,0,MAX_BUFF_SIZE);
        int n = recv(c->fd,buf,MAX_BUFF_SIZE,0);
        if(n<0){
            printf("接收数据失败");
            return;
        }
        if(n==0){
            break;
        }
        printf("%s",buf);
    }
    //关闭套接字
    shutdown(c->fd,SHUT_RDWR);
}

//发送get指令，获取服务器上的文件
void request_get(client *c){
    char buf[MAX_BUFF_SIZE];
    FILE* fp;
    //首先测试文件是否存在
    if(access(c->save_name,F_OK)==0){
        printf("failed to download %s: %s already exists\n",c->save_name,c->save_name);
        return;
    }
    //首先向客户端发送get指令
    char* cmd = (char*) malloc(sizeof(char) *(strlen(c->request_file)+4));
    //FIXME:这里没有向cmd填充内容
    sprintf(cmd,"get %s",c->request_file);
    if(send(c->fd,cmd,strlen(cmd),0)<0){
        printf("发送get指令失败");
        return;
    }
    //从服务器接收数据
    printf("start downloading %s...\n",c->request_file);
    fp = fopen(c->save_name,"w");

    if(fp==NULL){
        printf("打开文件%s失败",c->save_name);
        return;
    }

    while(1){
        memset(buf,0,MAX_BUFF_SIZE);
        int n = recv(c->fd,buf,MAX_BUFF_SIZE,0);
        if(n<0){
            printf("接收数据失败");
            return;
        }else if(n==0){
            printf("Ok %s",c->save_name);
            fclose(fp);
            break;
        }else{
            fwrite(buf,1,n,fp);
        }
    }
    shutdown(c->fd,SHUT_RDWR);
}
