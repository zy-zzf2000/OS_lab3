/*
 * @Author: zy 953725892@qq.com
 * @Date: 2023-01-11 17:38:53
 * @LastEditors: zy 953725892@qq.com
 * @LastEditTime: 2023-01-11 17:51:01
 * @FilePath: /lab3/server/daemon.h
 * @Description: 
 * 
 * Copyright (c) 2023 by zy 953725892@qq.com, All Rights Reserved. 
 */
#ifndef _DAEMON_H
#define _DAEMON_H

#include <sys/types.h>
#include <unistd.h>

void daemon(char* run_dir);

#endif