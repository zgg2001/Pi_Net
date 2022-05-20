/*************************************************************************
	> File Name: m_server.h
	> Author: ZHJ
	> Remarks: 服务端类 最外层
	> Created Time: Fri 20 May 2022 07:59:45 PM CST
 ************************************************************************/

#ifndef _M_SERVER_H_
#define _M_SERVER_H_

#include<unistd.h>
#include<sys/types.h>
#include<sys/epoll.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#include<string.h>
#include<errno.h>

#include"m_group_node.h"
#include"../public/log.h"

class m_server
{
public:
    using SOCKET = int;
    using task = std::function<void()>;
    static constexpr SOCKET INVALID_SOCKET = (SOCKET)(~0);
    static constexpr int SOCKET_ERROR = -1;

public:
    m_server();
    virtual ~m_server();
    m_server(const m_server&) = delete;
    m_server& operator= (const m_server&) = delete;

    /* 
    * 环境/数据初始化
    * 返回0为成功
    */
    int m_init();

    /*
    * 绑定ip/端口
    * 返回0为成功
    */
    int m_bind(const char* ip, unsigned short port);

    /*
    * 开始监听端口
    * n: 连接队列大小
    * 返回0为成功
    */
    int m_listen(int n);

    /*
    * 接受客户端连接
    * 返回0为成功
    */
    int m_accept();

    // 关闭server
    void m_close();

    // 启动子节点
    void m_start(int size);

    //开始工作
    void m_work();

private:
    //添加新客户端
    void addclient_main(m_client_node* client);

private:
    //服务器socket
    SOCKET _sock;

    //子线程
    std::vector<m_group_node*> _cell_servers;

    //task缓冲区相关
    std::list<task> _tasks;
    std::list<task> _tasksbuf;

    //lock
    std::mutex _mutex_task;//任务队列锁
    std::mutex _mutex_id;//主键锁
    std::mutex _mutex_gnode;//gnode锁
};

#endif
