/*************************************************************************
	> File Name: m_send_node.cpp
	> Author: ZHJ
	> Remarks: 客户端send节点 向服务端send
	> Created Time: Fri 20 May 2022 08:37:25 PM CST
 ************************************************************************/

#include"m_send_node.h"

m_send_node::m_send_node(m_client* client):
    _client(client)
{

}

m_send_node::~m_send_node()
{
    DEBUG("send_node ~ start");
    close_node();
    DEBUG("send_node ~ start");
}

void
m_send_node::start()
{
    DEBUG("send_node thread start");
    _thread.start(
        //create
        [this](m_thread*)
        {
            func_create(&_thread);
        },
        //run
        [this](m_thread*)
        {
            func_run(&_thread);
        },
        //destory
        [this](m_thread*)
        {
            func_destory(&_thread);
        }
    );
}

void
m_send_node::close_node()
{
    DEBUG("send_node thread close");
    _thread.close();
}

void 
m_send_node::func_create(m_thread* thread)
{
    DEBUG("send_node func_create() start"); 
}

void 
m_send_node::func_run(m_thread* thread)
{
    DEBUG("send_node func_run() start");
    
    //计时器->发心跳包
    m_timer timer;
    timer.update();
    double mytimer = 0.0;
    
    while(_thread.is_run())
    {
        //心跳包 两秒一发
        mytimer += timer.get_sec();
        timer.update();
        if(mytimer >= 2.0)
        {
            c2s_heart h;
            send(_sockfd, (const char*)&h, sizeof(h), 0);
            mytimer = 0.0;
        }
        
        //将缓冲区内数据加入 
        if(!_tasksbuf.empty()) 
        {
            std::lock_guard<std::mutex> lock(_mutex);
            for(auto& t : _tasksbuf)
            {
                _tasks.push_back(t);
            }
            _tasksbuf.clear();
        }
        
        //如果无任务
        if(_tasks.empty())
        {
            usleep(1000);//1ms
            continue;
        }
        
        //处理任务
        for(auto& t : _tasks)
        {
            t();
        }
        _tasks.clear();
    }
}

void 
m_send_node::func_destory(m_thread* thread)
{
    DEBUG("send_node func_destory() start");
}

void 
m_send_node::addtask(task t)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _tasksbuf.push_back(t);
}




