/*************************************************************************
	> File Name: m_server.cpp
	> Author: ZHJ
	> Remarks: 服务端类 最外层
	> Created Time: Fri 20 May 2022 07:59:45 PM CST
 ************************************************************************/

#include"m_server.h"

m_server::m_server():
    _sock(INVALID_SOCKET)
{
    _data_illu = -1;
    _data_temp = -1;
    _data_rh = -1;
    _data_db = -1;
}

m_server::~m_server()
{
    m_close();
    close(_sock);
}

int
m_server::m_init()
{
    //创建socket 
    if(_sock != INVALID_SOCKET)//存在socket
    {
        WARN("Server init warn -- socket:%d already exists", _sock);
        m_close();//m_server复位 
    }
    _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(_sock == INVALID_SOCKET)
    {
        ERROR("Server init faild -- %s", strerror(errno));
        return -1; 
    } 

    INFO("Server init(%d) success", _sock);
    return 0;
}

int
m_server::m_bind(const char* ip, unsigned short port)
{
    //无效套接字
    if(_sock == INVALID_SOCKET)
    {
        WARN("Server bind warn -- server socket not initialized");
        m_init();
    }
    
    //bind
    sockaddr_in _myaddr = {};
    _myaddr.sin_family = AF_INET;//IPV4
    _myaddr.sin_port = htons(port);
    if(ip == nullptr) 
        _myaddr.sin_addr.s_addr = htonl(INADDR_ANY);//监听ALL
    else
        inet_pton(AF_INET, ip, &_myaddr.sin_addr.s_addr);
    if(bind(_sock, (sockaddr*)&_myaddr, sizeof(sockaddr_in)) == SOCKET_ERROR)
    {
        ERROR("Server bind(%hd) faild -- %s", port, strerror(errno));
        close(_sock);
        _sock = INVALID_SOCKET;
        return -1;
    }
    else
    {
        INFO("Server bind(%hd) success", port);
        return 0;
    }
}

int
m_server::m_listen(int n)
{
    //无效套接字 
    if(_sock == INVALID_SOCKET)
    {
        ERROR("Server listen(%d) faild -- server socket not initialized", n);
        return -1;
    }
    
    //开始监听
    if(listen(_sock, n) == SOCKET_ERROR) 
    {
        ERROR("Server listen(%d) faild -- %s", n, strerror(errno));
        close(_sock);
        _sock = INVALID_SOCKET;
        return -2;
    }
    else
    {
        INFO("Server listen(%d) success", n);
        return 0;
    }
}

int
m_server::m_accept()
{
    sockaddr_in clientAddr = {};
    int addr_len = sizeof(sockaddr_in);
    SOCKET temp_socket = INVALID_SOCKET;
    
    //accept
    temp_socket = accept(_sock, (sockaddr*)&clientAddr, (socklen_t*)&addr_len);
    if(temp_socket == INVALID_SOCKET)
    {
        ERROR("Server accept(%d) error -- %s", temp_socket, strerror(errno));
        return -1;
    }
    
    //连接后分配给登录节点
    addclient_main(new m_client_node(temp_socket));

    INFO("Server accept(%d) success from %s", 
         temp_socket, inet_ntoa(clientAddr.sin_addr));
    return 0;
}

void
m_server::m_close()
{
    DEBUG("Server m_close start");
}

void
m_server::m_start(int size)
{
    for (int n = 0; n < size; ++n)
	{
		//线程加入容器 
		auto ser = new m_group_node(n, this);
		_cell_servers.push_back(ser);
        INFO("Server login_node(%d) start", n);
	}
}

void
m_server::m_work()
{
    INFO("Server work start");
    
    //无效套接字 
    if(_sock == INVALID_SOCKET)
    {
        ERROR("Server work faild -- server socket not initialized");
        return;
    }

    //epoll
    int epollfd = epoll_create(1);//主线程epoll仅关注连接事件
    struct epoll_event ev;
    ev.data.fd = _sock;
    ev.events = EPOLLIN;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, _sock, &ev);

    //处理
    while(1)
    {
        //epoll监听新连接
        struct epoll_event events;
        int event_count = epoll_wait(epollfd, &events, 1, 1); 
        if(event_count < 0)
        {
            ERROR("Server epoll_wait() error -- %s", strerror(errno));
            break;
        }
        else if(event_count > 0)//处理新连接
        {
            m_accept();
        }
        
        //数据库更新task缓冲区 
        if(!_tasksbuf.empty()) 
        {
            std::lock_guard<std::mutex> lock(_mutex_task);
            for(auto& t : _tasksbuf)
            {
                _tasks.push_back(t);
            }
            _tasksbuf.clear();
        }
        
        //处理数据库更新任务
        if(_tasks.empty())
            continue;
        for(auto& t : _tasks)
        {
            t();
        }
        _tasks.clear();
    }
    
    close(epollfd);
    INFO("Server work finish");
}

void 
m_server::addclient_main(m_client_node* client)
{
    //找出客户端最少的线程 然后加入 
	auto pMinServer = _cell_servers[0];
	for(auto pCellServer : _cell_servers)
	{
		if (pMinServer->get_client_num() > pCellServer->get_client_num())
		{
			pMinServer = pCellServer;
		}
	}
	pMinServer->addclient(client);
}