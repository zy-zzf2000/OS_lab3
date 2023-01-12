/*
 * @Author: zy 953725892@qq.com
 * @Date: 2022-11-16 16:28:01
 * @LastEditors: zy 953725892@qq.com
 * @LastEditTime: 2023-01-12 23:41:31
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
        exit(1);
    }
    //从服务器接收数据
    while(1){
        memset(buf,0,MAX_BUFF_SIZE);
        int n = recv(c->fd,buf,MAX_BUFF_SIZE,0);
        if(n<0){
            printf("接收数据失败");
            exit(1);
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
    
    //首先向客户端发送size指令
    request_size(c);

    //首先向客户端发送get指令
    char* cmd = (char*) malloc(sizeof(char) *(strlen(c->request_file)+4));
    //FIXME:这里没有向cmd填充内容(Done)
    sprintf(cmd,"get %s",c->request_file);
    if(send(c->fd,cmd,strlen(cmd),0)<0){
        printf("发送get指令失败");
        exit(1);
    }
    //从服务器接收数据
    printf("start downloading %s...\n",c->request_file);
    fp = fopen(c->save_name,"w");

    if(fp==NULL){
        printf("打开文件%s失败",c->save_name);
        exit(1);
    }

    //TODO:打印进度条(Done)
    int finished = 0;  //finished代表当前已经下载的百分比
    int recv_bytes = 0;
    char proc[102];  //用户打印进度条的#
    bar_print(0,2,proc);

    int is_close = 0;

    while(1){
        memset(buf,0,MAX_BUFF_SIZE);
        int n = recv(c->fd,buf,MAX_BUFF_SIZE,0);
        if(n<0){
            printf("接收数据失败");
            exit(1);
        }else if(n==0){
            bar_print(finished,100,proc);
            printf("\nOk %s",c->save_name);
            if(is_close==0){
                fclose(fp);
                is_close = 1;
            }
            break;
        }else{
            if(strncmp(buf,"unzip",5)==0){
                //说明文件已经传输完毕，需要解压
                bar_print(finished,100,proc);
                if(is_close==0){
                    fclose(fp);
                    is_close = 1;
                }
                char cmd[50];
                //首先重命名压缩包后缀
                sprintf(cmd,"mv %s %s.tar",c->save_name,c->save_name);
                system(cmd);
                //解压
                memset(cmd,0,50);
                sprintf(cmd,"tar -xf %s.tar",c->save_name);
                system(cmd);
                //重命名为目标后缀
                if(strcmp(c->save_name,c->request_file)!=0){
                    memset(cmd,0,50);
                    sprintf(cmd,"mv %s %s",c->request_file,c->save_name);
                    system(cmd);
                }
                //删除压缩包
                memset(cmd,0,50);
                sprintf(cmd,"%s.tar",c->save_name);
                remove(cmd);
                printf("\nOk %s",c->save_name);
                break;
            }else if(strncmp(buf,"error",5)==0){
                printf("failed to download %s: %s",c->request_file,buf+6);
                if(is_close==0){
                    fclose(fp);
                    is_close = 1;
                }
                remove(c->save_name);
                break;
            }else{
                //TODO:buf中包含0,因此不能用strstr判断是否有终结符
                int lastNum = check_spilt(buf,n);
                if(lastNum!=-1){
                    recv_bytes += lastNum;
                    int new_finished = (int) (recv_bytes*100.0/c->file_size);
                    bar_print(finished,new_finished,proc);
                    finished = new_finished;
                    fwrite(buf,1,lastNum,fp);
                    if(strlen(buf+lastNum+2)!=0){
                        //说明是文件夹，还需要进行解压缩操作
                        bar_print(finished,100,proc);
                        if(is_close==0){
                            fclose(fp);
                            is_close = 1;
                        }
                        char cmd[50];
                        //首先重命名压缩包后缀
                        sprintf(cmd,"mv %s %s.tar",c->save_name,c->save_name);
                        system(cmd);
                        //解压
                        memset(cmd,0,50);
                        sprintf(cmd,"tar -xf %s.tar",c->save_name);
                        system(cmd);
                        //重命名为目标后缀
                        if(strcmp(c->save_name,c->request_file)!=0){
                            memset(cmd,0,50);
                            sprintf(cmd,"mv %s %s",c->request_file,c->save_name);
                            system(cmd);
                        }
                        //删除压缩包
                        memset(cmd,0,50);
                        sprintf(cmd,"%s.tar",c->save_name);
                        remove(cmd);
                    }
                }else{
                    recv_bytes += n;
                    int new_finished = (int) (recv_bytes*100.0/c->file_size);
                    bar_print(finished,new_finished,proc);
                    finished = new_finished;
                    fwrite(buf,1,n,fp);
                }
            }
        }
    }
    shutdown(c->fd,SHUT_RDWR);
}

void request_size(client *c){
    char buf[MAX_BUFF_SIZE];
    FILE* fp;
    //首先测试文件是否存在
    if(access(c->save_name,F_OK)==0){
        printf("failed to download %s: %s already exists\n",c->save_name,c->save_name);
        exit(1);
    }
    //首先向客户端发送size指令
    char* cmd = (char*) malloc(sizeof(char) *(strlen(c->request_file)+5));
    sprintf(cmd,"size %s",c->request_file);
    if(send(c->fd,cmd,strlen(cmd),0)<0){
        printf("发送get指令失败");
        exit(1);
    }
    //从服务器接收文件大小
    int n = recv(c->fd,buf,MAX_BUFF_SIZE,0);
    if(strncmp(buf,"error",5)==0){
        printf("failed to download %s: %s\n",c->request_file,buf+6);
        exit(1);
    }
    if(n<0){
        printf("接收文件长度失败");
        exit(1);
    }
    int file_size = atoi(buf);
    c->file_size = file_size;
    printf("%s的大小为:%d\n",c->request_file,file_size);
}

void bar_print(int finished,int cnt,char* proc){
    int i;
    for(i=finished;i<cnt;i++){
        proc[i] = '#';
    }
    // printf("\n");
    // printf("fflush之前");
    printf("[%-100s] [%d%%]\r", proc, cnt);
    fflush(stdout);
    // usleep(10000);
    // printf("f");
}

int check_spilt(char* buf,int len){
    int i;
    for(i=0;i<len;i++){
        if(buf[i]=='$'&&buf[i+1]=='_'){
            return i;
        }
    }
    return -1;
}