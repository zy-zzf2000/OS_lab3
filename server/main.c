/*
 * @Author: zy 953725892@qq.com
 * @Date: 2022-11-16 02:09:33
 * @LastEditors: zy 953725892@qq.com
 * @LastEditTime: 2023-01-11 22:26:50
 * @FilePath: /lab3/server/main.c
 * @Description: server主函数
 * 
 * Copyright (c) 2022 by zy 953725892@qq.com, All Rights Reserved. 
 */
#include <getopt.h>

#include "server.h"
#include "daemon.h"

char *ip = "127.0.0.1";
ushort port = 12345;
int show_hide = 0;
int show_help = 0;
int show_subdir = 0;
int daemon_mode = 0;

int main(int argc, char* argv[]){
    //处理命令行参数
    int options;
    int long_option_idx;
    char* optstring = "l:Hp:dhs";
    static struct option long_options[] = {
        {"listen",  required_argument, 0,  'l' },
        {"port",    required_argument, 0,  'p' },
        {"help",    no_argument,       0,  'h' },
        {"daemon",  no_argument,       0,  'd' },
        {"hidden",  no_argument,       0,  'H' },
        {"subdir",  no_argument,       0,  's' },
        {0,         0,                 0,   0  },
    };
    
    while((options = getopt_long(argc, argv, optstring, long_options, &long_option_idx)) != -1){
        switch(options){
            case 'l':
                ip = optarg;
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 'H':
                show_hide = 1;
                break;
            case 'h':
                show_help = 1;
                break;
            case 'd':
                daemon_mode = 1;
                break;
            case 's':
                show_subdir = 1;
                break;
            default:
                printf("err command!\n");
                return 0;
                break;
        }
    }

    if(show_help == 1){
        printf("Usage: fserver [OPTION] ... <DIR>\n");
        printf("serve some files over tcp \n");
        printf("\n");
        printf("args:\n");
        printf("    <DIR> Which directory to serve\n");
        printf("\n");
        printf("options:\n");
        printf("    -l, --listen=ADDR         specify source address to use [default is localhost]\n");
        printf("    -H, --hidden              show hidden file\n");
        printf("    -p, --port=PORT           specify listening port [default port is 12345]\n");
        printf("    -h, --help                display this help and exit\n");
        printf("    -d, --daemon              run as daemon\n");
        printf("    -s, --subdir              show subdir\n");
        return 0;
    }

    char* dir_root = argv[optind];
    if(dir_root == NULL){
        printf("err command!\n");
        printf("Usage: fserver [OPTION] ... <DIR>\n");
        return 0;
    }

    if(daemon_mode == 1){
        daemonize(dir_root);
    }
    server s;
    init_server(&s, ip, port, dir_root);
    show_dir(&s,show_hide,show_subdir);
    do_server(&s);
    return 0;
}