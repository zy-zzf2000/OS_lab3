/*
 * @Author: zy 953725892@qq.com
 * @Date: 2022-11-16 17:06:33
 * @LastEditors: zy 953725892@qq.com
 * @LastEditTime: 2022-11-16 17:45:23
 * @FilePath: /lab3/client/main.c
 * @Description: 
 * 
 *
 * 
 * Copyright (c) 2022 by zy 953725892@qq.com, All Rights Reserved. 
 */
#include <getopt.h>

#include "request.h"

char *server_ip = "127.0.0.1";
ushort server_port = 12345;
int list_files = 1;
int show_help = 0;
char* request_file = NULL;
char* save_name = NULL;

int main(int argc, char* argv[]){
    //解析命令行参数
        //处理命令行参数
    int options;
    int long_option_idx;
    char* optstring = "ls:p:d:o:h";
    static struct option long_options[] = {
        {"list",    no_argument,       0,  'l' },
        {"server",  required_argument, 0,  's' },
        {"port",    required_argument, 0,  'p' },
        {"dl",      required_argument, 0,  'd' },
        {"output",  required_argument, 0,  'o' },
        {"help",    no_argument,       0,  'h' },
        {0,         0,                 0,   0  } 
    };
    
    while((options = getopt_long(argc, argv, optstring, long_options, &long_option_idx)) != -1){
        switch(options){
            case 'l':
                list_files = 1;
                break;
            case 's':
                server_ip = optarg;
                break;
            case 'p':
                server_port = atoi(optarg);
                break;
            case 'd':
                request_file = optarg;
                break;
            case 'o':
                save_name = optarg;
                break;
            case 'h':
                show_help = 1;
                break;
            default:
                printf("err command!\n");
                return 0;
                break;
        }
    }

    if(show_help == 1){
        printf("Usage: fclient [OPTION] ...\n");
        printf("download file from fserver over tcp \n");
        printf("\n");
        printf("options:\n");
        printf("    -l, --list                 show remote served files [default action]\n");
        printf("    -s, --server            specify server address [default addr is localhost]\n");
        printf("    -p, --port                 specify server port [default port is 12345]\n");
        printf("    -d, --dl=FILE              download specified file\n");
        printf("    -o, --output=FILE        output to the FILE\n");
        printf("    -h, --help                display this help and exit\n");
        return 0;
    }

    client c;
    init_client(&c, server_ip, server_port,request_file,save_name);
    //如果没有指定下载文件，则只需要列出服务器上的文件
    if(request_file == NULL){
        request_show(&c);
    }else{
        request_get(&c);
    }

    return 0;
}