#include <iostream>
#include <string.h>
#include "Server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/epoll.h>

using namespace std;

Server::Server()
{
    int socket_fd, conn_fd;
    sockaddr_in addr;

    bzero(&addr,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);

    if (inet_pton(AF_INET, ip, &addr.sin_addr) == -1)
    {
        cout << "inet_pton error." << endl;
        return;
    }

    //创建套接字
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        cout << "create socket_fd error." << endl;
        return;
    }

    //绑定地址信息
    if ((bind(socket_fd, (sockaddr *)&addr, sizeof(addr))) == -1)
    {
        cout << "bind error." << endl;
        return;
    }

    //监听套接字
    if (listen(socket_fd, 20) == -1)
    {
        cout << "listen error." << endl;
        return;
    }

    int epoll_fd;
    epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = socket_fd;

    //创建epoll
    if ((epoll_fd = epoll_create(10)) == -1)
    {
        cout << "create epoll error." << endl;
        return;
    }

    //添加fd
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event) == -1)
    {
        cout << "epoll_ctl error." << endl;
        return;
    }

    epoll_event events[5];
    while (true)
    {
        int ret_num = epoll_wait(epoll_fd, events, 5, -1);
        if (ret_num == -1)
        {
            cout << "epoll_wait error." << endl;
            return;
        }
        cout<<"event_num:"<<ret_num<<endl;
        for (int i = 0; i < ret_num; i++)
        {
            if (events[i].data.fd == socket_fd)
            {
                if ((conn_fd = accept(socket_fd, NULL, NULL)) == -1)
                {
                    cout << "accept error." << endl;
                }
                event.data.fd = conn_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn_fd, &event) == -1)
                {
                    cout << "epoll_ctl error." << endl;
                    return;
                }
            }
            else if(events[i].events)
            {
                char buffer[3];
                bzero(buffer,sizeof(buffer));
                conn_fd=events[i].data.fd;
                int n = recv(conn_fd, buffer, sizeof(buffer), 0);

                if(n>0)
                {
                    cout<< buffer << endl;
                }
                else
                {
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, &event);
                }
                
            }
        }
    }
    close(socket_fd);
}