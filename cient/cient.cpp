#include<iostream>
#include<string.h>
#include"Cient.h"
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string>
#include<sys/wait.h>

using namespace std;

#define MAX_MESSAGE_LENGTH

void Cient::sendData()
{
    int socket_fd,bind_ret;
    string buffer_in;

    //地址信息初始化
    sockaddr_in addr;
    bzero(&addr,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    if(inet_pton(AF_INET,ip,&addr.sin_addr)==-1)
    {
        cout<<"inet_pton error."<<endl;
        return;
    }

    //创建TCP套接字
    if((socket_fd=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        cout<<"create socket error."<<endl;
        return ;
    }

    //连接服务器  
    if(connect(socket_fd,(sockaddr*)&addr,sizeof(addr))==-1)
    {
        cout<<"connect error."<<endl;
        cout<<errno<<endl;
        return ;
    }
    //cout<<"please input:"<<endl;
    //cin.getline(buffer_in,100);
    buffer_in="hello world.";
    //发送数据
    if(send(socket_fd,(char*)buffer_in.data(),buffer_in.size(),0)==-1)
    {
        cout<<"send error."<<endl;
        return ;
    }
    // sleep(0.5);
    // buffer_in=" world.";
    // if(send(socket_fd,(char*)buffer_in.data(),buffer_in.size(),0)==-1)
    // {
    //     cout<<"send error."<<endl;
    //     return ;
    // }
    close(socket_fd);  
}