/*
 * @Author: zy 953725892@qq.com
 * @Date: 2022-11-16 16:10:35
 * @LastEditors: zy 953725892@qq.com
 * @LastEditTime: 2022-11-16 16:21:27
 * @FilePath: /lab3/client/request.h
 * @Description: 定义客户端可以发送的请求
 * 
 * Copyright (c) 2022 by zy 953725892@qq.com, All Rights Reserved. 
 */
#ifndef _REQUEST_H
#define _REQUEST_H

#include "client.h"

//发送show指令，获取服务器上的文件列表
void request_show(client *c);

//发送get指令，获取服务器上的文件
void request_get(client *c);

#endif