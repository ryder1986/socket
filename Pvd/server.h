#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <thread>
#include <functional>
#include <sys/ioctl.h>
using namespace std;
#define BUF_SIZE 10
#define IP_STR_LEN 20
#include <cstring>
#include <algorithm>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <errno.h>
//#include <tcp.h>
#include <sys/ioctl.h>
//#include "common.h"
//#include "zenlog.h"

#include <vector>
//using namespace std;
#define	TCP_NODELAY	 1
class Socket{
public:
    static  int close_socket(int *s)
    {
        shutdown(*s,SHUT_RDWR);
        close(*s);
        *s=-1;
    }

    static   int StartTcpServerSock(unsigned short port, int timeoutsec,int maxqueue)
    {
        int fd;
        int value;
        struct timeval timeo;
        socklen_t len = sizeof(timeo);

        memset(&timeo, 0, sizeof(timeo));
        if((fd= socket(AF_INET,SOCK_STREAM,0)) < 0){
            //prt(err,"socket err");
            return -1;
        }
        value = 1;
        if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value))<0){
            //prt(err,"socket err");
            close(fd);
            return -1;
        }

        struct sockaddr_in sa;
        memset(&sa, 0, sizeof(struct sockaddr_in));
        sa.sin_family = AF_INET;
        sa.sin_addr.s_addr = htonl(INADDR_ANY);
        sa.sin_port = htons(port);
        if(bind(fd, (struct sockaddr *)(&sa),sizeof(struct sockaddr))<0){
            //prt(err,"socket err,bind err");
            close(fd);
            return -1;
        }

        if(listen(fd, maxqueue)<0)
        {
            //prt(err,"socket err");
            close(fd);
            return -1;
        }
        return fd;
    }

    static    int CreatBroadcast(int port)
    {
        int socket_fd;
        struct sockaddr_in user_addr;

        user_addr.sin_family=AF_INET;
        user_addr.sin_port=htons(port);
        user_addr.sin_addr.s_addr=htonl(INADDR_ANY);
        if((socket_fd=(socket(AF_INET,SOCK_DGRAM,0)))==-1){
            perror("socket");
            return -1;
        }

        int so_broadcast=1;
        setsockopt(socket_fd,SOL_SOCKET, SO_BROADCAST, &so_broadcast,sizeof(so_broadcast));
        if((bind(socket_fd,(struct sockaddr *)&user_addr,sizeof(struct sockaddr)))==-1){
            perror("bind");
            return -1;
        }
        return socket_fd;
    }

    static   int RecvFromBroadcast(unsigned char *buffer, int len, int sock)
    {
        return recvfrom(sock,buffer,len,0,NULL,NULL);
    }

    static   int WaitTcpConnect(int sock, unsigned long sec, char * ip, unsigned short * port)
    {
        int cl_fd=0, one;
        fd_set rd_set;
        struct timeval timeout;
        struct sockaddr_in  client_addr;
        int client_len = sizeof(struct sockaddr_in);

        timeout.tv_sec = sec;
        timeout.tv_usec = 0;

        FD_ZERO(&rd_set);
        FD_SET(sock, &rd_set);
        while ((cl_fd = select(sock + 1, &rd_set, NULL, NULL, &timeout)) < 0) {

            if (EINTR == errno) {
                //prt(err, "socket err EINTR");

            }
            if (EINVAL == errno) {
                //prt(err, "socket err EINVAL");

            }
            if (ENOMEM == errno) {
                //prt(err, "socket err ENOMEM");

            }
            if (EBADF == errno) {
                //prt(err, "socket err EBADF");

            }
            //prt(err, "socket err,need rebind socket");
            cl_fd = -1;
            return cl_fd;
        }
        //  	//prt(net,"select rst %d",cl_fd);
        //	if(0== cl_fd){
        //	   	//prt(err,"select time out,need select again  fd %d",cl_fd);
        //	return 0;
        //	}

        if(FD_ISSET(sock, &rd_set))
        {
            //	//prt(net,"get msg from client");
#if 0
            while((cl_fd = accept(sock, (struct sockaddr *)&client_addr, (socklen_t *)&client_len))<0){
                if(EINTR==errno){
                    //Log0("accept continue: cl_fd[%d] errno:[%s]", cl_fd, strerror(errno));
                    continue;
                }
                else{
                    //Log0("accept error: cl_fd[%d] errno:[%s]", cl_fd, strerror(errno));
                    return 0;
                }
            }
#else
            if((cl_fd = accept(sock, (struct sockaddr *)&client_addr, (socklen_t *)&client_len))<=0){

                //prt(err,"accept  err  %d",cl_fd);
                close_socket(&sock);

                cl_fd=-1;
            }else{
                //		  	//prt(net,"accept rst %d",cl_fd);
            }
#endif
#if 0
#if 0
            one = 1;
            if(setsockopt(cl_fd, IPPROTO_TCP, TCP_NODELAY,
                          (char *)&one, sizeof(one))<0){
                close(cl_fd);
                //Log0("setsockopt error: cl_fd[%d]", cl_fd);
                return 1;
            }
#else
            //设置发送超时
            //设置接收超时
            struct timeval timeout1 = {0,450000};
            if((setsockopt(cl_fd, SOL_SOCKET,SO_SNDTIMEO, (char *)&timeout1, sizeof(struct timeval))<0)
                    || (setsockopt(cl_fd, SOL_SOCKET,SO_RCVTIMEO, (char *)&timeout1, sizeof(struct timeval))<0)){
                close(cl_fd);
                //Log0("setsockopt error: cl_fd[%d] errno:[%s]", cl_fd, strerror(errno));
                return 1;
            }
#endif
#endif

            if(ip != NULL)
                strcpy(ip, inet_ntoa(client_addr.sin_addr));
            if(port != NULL)
                *port = ntohs(client_addr.sin_port);
            // return cl_fd;
        }else
        {
            //prt(net,"client time out ,returning %d ",cl_fd);
        }
        return cl_fd;
        //   return 1;
    }

    static   int ConnectTcpClient(int sock, char * ip, unsigned short port)
    {
        struct sockaddr_in server_addr;
        memset(&server_addr, 0 , sizeof(struct sockaddr_in));
        server_addr.sin_addr.s_addr = inet_addr(ip);
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);

#if 0
        int error=-1, len, one;
        len = sizeof(int);
        timeval tm;
        fd_set set;
        unsigned long ul = 1;
        ioctl(sock, FIONBIO, &ul); //设置为非阻塞模式
#endif



        fd_set set;
        unsigned long ul = 1;
        ioctl(sock, FIONBIO, &ul); //设置为非阻塞模式

        ul = 0;
        ioctl(sock, FIONBIO, &ul); //设置为阻塞模式

        //	int save_mode = fcntl(sock, F_GETFL, 0);
        //	fcntl(sock, F_SETFL, save_mode | O_NONBLOCK);//设置为阻塞模式

        if(connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1){
            {
                //    		ul = 0;
                //    	 	ioctl(sock, FIONBIO, &ul); //设置为 阻塞模式

                return -1;
            }
#if 0
            tm.tv_sec = 1;
            tm.tv_usec = 0;
            FD_ZERO(&set);
            FD_SET(sock, &set);
            if(select(sock+1, NULL, &set, NULL, &tm)>0){
                getsockopt(sock, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
                if(error != 0)
                    return -1;
            } else
                return -1;
#if 0
            one = 1;
            if(setsockopt(sock, IPPROTO_TCP, TCP_NODELAY,
                          (char *)&one, sizeof(one))<0){
                close(sock);
                return 1;
            }
#endif
#endif
        }

#if 0
        ul = 0;
        ioctl(sock, FIONBIO, &ul); //设置为阻塞模式
#endif
        return 1;
    }

    static   int RecvDataByTcp(int sock,char* buffer,int len)
    {
        //prt(info,"get buffer len : %d",len);
        int offset=0;
        while(len>0){
            int re = recv(sock,buffer+offset,len,0);
            if(re<=0){
                if(re<0 && EINTR == errno){
                    //Log0("recv continue: re[%d] errno:[%s]", re, strerror(errno));
                    continue;
                }
                else{
                    //Log0("recv error: re[%d] errno:[%s]", re, strerror(errno));
                    return offset;
                }
            }
            len -= re;
            offset += re;
        }
        return offset;
    }

    static   int RecvDataByTcp1(int sock,char* buffer,int len)
    {
        //prt(info,"get buffer len : %d",len);
        int offset=0;
        while(len>0){
            int re = recv(sock,buffer+offset,len,0);
            if(re<=0){
                if(re<0 && EINTR == errno){
                    //Log0("recv continue: re[%d] errno:[%s]", re, strerror(errno));
                    continue;
                }
                else{
                    //Log0("recv error: re[%d] errno:[%s]", re, strerror(errno));
                    return offset;
                }
            }
            offset += re;
            return offset;
            //        len -= re;
            //        offset += re;
        }
        return offset;
    }

    //int RecvDataByTcp(int sock,char* buffer,int len){
    //int re = recv(sock,buffer+offset,len,0);

    //}
    static    int SendDataByTcp(int sock,const char * buffer, int len)
    {
        //	//prt(info,"send buffer len : %d",len);
        int ret = send(sock, buffer, len, 0);
        if(ret<=0){
            return -1;
        }
        return ret;
    }

#if 0
    int SendDataByTcp(int sock, char * buffer, int len)
    {
        int ret,lens = 0;
        while(1){
            ret = send(sock, buffer+lens, len-lens, 0);
            if(ret<=0){
                if(ret<0 && EINTR == errno){
                    //Log0("send continue: re[%d] errno:[%s]", ret, strerror(errno));
                    continue;
                }
                else{
                    //Log0("send error: ret[%d] errno:[%s]", ret, strerror(errno));
                    return ret;
                }
            }
            else{
                lens += ret;
                if(lens == len)
                    break;
            }
        }
        return lens;
    }
#endif

    static    int SendDataByClient(char*buffer, int len, char* ip, int port)
    {
        int sc = CreateTcpClientSock(0, 1);
        if(sc==-1)
            return sc;

        int ret = ConnectTcpClient(sc, ip, port);
        if(ret==-1)
        {
            close(sc);
            return -1;
        }

        ret = SendDataByTcp(sc, buffer, len);
        close(sc);
        return ret;
    }

    static    int CreateTcpClientSock(unsigned short port, int timeoutsec)
    {
        int fd;
        int value;
        struct timeval timeo;
        socklen_t len = sizeof(timeo);

        memset(&timeo, 0, sizeof(timeo));
        if((fd =socket(AF_INET, SOCK_STREAM, 0))<0)
            return -1;

        value = 1;
        if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &value , sizeof(value))<0){
            close(fd);
            return -1;
        }

        struct sockaddr_in sa;
        memset(&sa, 0, sizeof(struct sockaddr_in));
        sa.sin_family = AF_INET;
        sa.sin_addr.s_addr = INADDR_ANY;
        sa.sin_port = htons(port);
        if(bind(fd, (struct sockaddr *)(&sa), sizeof(struct sockaddr))<0){

            close(fd);
            return -1;
        }
        return fd;
    }

    static   int UdpCreateSocket(unsigned short port)
    {
        int sockfd;
        struct sockaddr_in my_addr;
        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0))<0){
            return -1;
        }

        my_addr.sin_family = AF_INET;
        my_addr.sin_port = htons(port);
        my_addr.sin_addr.s_addr = INADDR_ANY;
        if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))<0){
            // print_net("bind port %d err",port);
            return -1;
        }else{
            // print_net("bind port %d ok",port);
        }
        return sockfd;
    }

    static   int UdpSendData(int sock, char * ip, unsigned short port, char * buf, int slen)
    {
        struct sockaddr_in their_addr;
        their_addr.sin_family = AF_INET;
        their_addr.sin_port = htons(port);
        inet_pton(AF_INET, ip, &their_addr.sin_addr);
        int ret,sended = 0;
        int len = slen;
        while(1){
            ret = sendto(sock, buf+sended, len, 0,(struct sockaddr*)&their_addr, sizeof(struct sockaddr));
            if(ret == -1){
                break;
            }
            else{
                sended += ret;
                len    -= ret;
                if(sended == slen)break;
            }
        }
        return sended;
    }
};

class Session{
public:
    Session(int fd,string ip,int port):quit(false),client_ip(ip),
        client_port(port)
    {
        skt=fd;
        int ul = 0;
        ioctl(fd, FIONBIO, &ul);
        cout<<"new connection"<<endl;
        auto func_recv=bind(&Session::recv,this);

        trd=new thread([func_recv](){func_recv();});
    }
    ~Session()
    {
        cout<<"session deleteing "<<ip()<<endl;
        quit=true;
        trd->join();
        delete trd;

    }

    int send(const char *buf,int len)
    {
        int ret= Socket::SendDataByTcp(skt,buf,len);
        if(ret){
            cout<<"send  "<<ret<<" bytes"<<endl;
        }else{
            cout<<"send  fail"<<endl;
        }
    }
    int recv()
    {
        while(!quit){
            int ret=Socket::RecvDataByTcp1(skt,buf,BUF_SIZE);
            if(ret){
                cout<<"read  "<<ret<<" bytes"<<endl;
                process_data(this,buf,ret);
            }else{
                cout<<"ret  "<<ret<<endl;
                break;
                cout<<"socket maybe closed,retry read after 1sec"  <<endl;
                this_thread::sleep_for(chrono::seconds(1));
            }

        }
        end_this(this);
    }
    string ip()
    {
        return client_ip;
    }
    int port()
    {
        return client_port;
    }
private:
    int skt;
    bool quit;
    char buf[BUF_SIZE];
    std::thread *trd;
    string client_ip;
    int client_port;
public:
    function <void(Session *)> end_this=[](Session *){ cout<<"we  can quit now"<<endl;};
    function <void(Session *,char *data,int len)> process_data=[](Session *,char *data,int len){ cout<<"process data"<<endl;};

};

class Tcpserver{
    function <void(Session *,char *,int)>fct;
public:
    Tcpserver(int p):port(p),quit(false)
    {
        fd= Socket::StartTcpServerSock(port,1000,1000);
        auto func=bind(&Tcpserver::listen,this);
        trd=new thread([func](){func();});
    }
    Tcpserver(int p,function <void(Session *,char *,int)>fc):port(p),quit(false),fct(fc)
    {
        fd= Socket::StartTcpServerSock(port,1000,1000);
        auto func=bind(&Tcpserver::listen,this);
        trd=new thread([func](){func();});
    }
    Tcpserver(vector<Session*> *clts,int p,function <void(Session *,char *,int)>fc):port(p),quit(false),fct(fc)
    {
        fd= Socket::StartTcpServerSock(port,1000,1000);
        auto func=bind(&Tcpserver::listen,this);
        trd=new thread([func](){func();});
    }
    ~Tcpserver()
    {
        quit=true;
        trd->join();
        int sz=clients.size();
        for(int i=sz-1;i>=0;i--)
        {
            delete clients[i];
            clients.pop_back();
        }
        delete trd;
    }

    void listen()
    {
        while(!quit){

            memset(peer_ip,0,IP_STR_LEN);
            int cl_fd=Socket::WaitTcpConnect(fd,1,peer_ip,&peer_port);
            if(cl_fd>0){
                cout<<"get connect from "<<peer_ip<<":"<<peer_port<<endl;
                Session *s=new Session(cl_fd,peer_ip,peer_port);
                s->end_this=bind(&Tcpserver::quit_session,this,placeholders::_1);
                s->process_data=fct;
                clients.push_back(s);
            }
        }
    }
    void send_test()
    {
        if(clients.size())
        {
            clients[0]->send("123456",6);
        }
    }
//    void set_ret_func(function <void(char *,int )> func)
//    {
//        for(int i=0;i<clients.size();i++){
//            clients[i]->process_data=func;
//        }
//    }

    void size()
    {
        cout<<clients.size();
    }
private:
    void quit_session(Session *s)
    {
        vector <Session *>::iterator e=find(clients.begin(),clients.end(),s);
        clients.erase(e);
        thread([s](){
            delete s;
        }).detach();
    }

    bool quit;
    int port;
    std::thread *trd;
    unsigned short peer_port;
    int fd;
    char peer_ip[IP_STR_LEN];
public:
    vector <Session *> clients;

};



#endif // SERVER_H
