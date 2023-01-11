#include "daemon.h"

void daemonize(char* run_dir){
    pid_t pid;
    pid = fork();
    if(pid<0){
        printf("fork error");
        exit(1);
    }else if(pid>0){
        //父进程直接退出
        exit(0);
    }
    //脱离原本会话
    setsid();
    chdir(run_dir);
    umask(0);
    //删除守护进程的所有文件描述符
    int i = 0;
    for(i=0;i<getdtablesize();i++){
        close(i);
    }
}