#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <strings.h>
#include <fcntl.h>
#include <cerrno>

#define MAX_EVENTS 10
#define PORT 8080

//设置socket连接为非阻塞模式
void setnonblocking(int sockfd)
{
    int opts;

    opts = fcntl(sockfd, F_GETFL);
    if(opts < 0)
    {
        perror("fcntl(F_GETFL)\n");
        exit(1);
    }
    opts = (opts | O_NONBLOCK);
    if(fcntl(sockfd, F_SETFL, opts) < 0)
    {
        perror("fcntl(F_SETFL)\n");
        exit(1);
    }
}

int main()
{
    struct epoll_event ev, events[MAX_EVENTS];
    int addrlen, listenfd, conn_sock, nfds, epfd, fd, i, nread;
    ssize_t n;
    struct sockaddr_in local, remote;
    char buf[BUFSIZ];

    //创建listen socket
    bzero(&local, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_port = htons(PORT);

    listenfd = socket(local.sin_family, SOCK_STREAM, IPPROTO_TCP);

    setnonblocking(listenfd);

    if( bind(listenfd, (struct sockaddr *) &local, sizeof(local)) < 0)
    {
        perror("bind\n");
        exit(1);
    }
    listen(listenfd, 20);//设置为监听套接字

    epfd = epoll_create(MAX_EVENTS);

    ev.events = EPOLLIN;
    ev.data.fd = listenfd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev) == -1)
    {
        perror("epoll_ctl: listen_sock");
        exit(EXIT_FAILURE);
    }

    for (;;)
    {
        nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);//超时时间-1，永久阻塞直到有事件发生
        if (nfds == -1)
        {
            perror("epoll_pwait");
            exit(EXIT_FAILURE);
        }

        for (i = 0; i < nfds; ++i)
        {
            fd = events[i].data.fd;

            if (fd == listenfd) //如果是监听的listenfd，那就是连接来了，保存来的所有连接
            {
                //每次处理一个连接，while循环直到处理完所有的连接
                while ((conn_sock = accept(listenfd, (struct sockaddr *) &remote,
                                           (socklen_t *)&addrlen)) > 0)
                {
                    setnonblocking(conn_sock);
                    ev.events = EPOLLIN | EPOLLET;//边沿触发非阻塞模式
                    ev.data.fd = conn_sock;
                    //把连接socket加入监听结构体
                    if (epoll_ctl(epfd, EPOLL_CTL_ADD, conn_sock,
                                  &ev) == -1) {
                        perror("epoll_ctl: add");
                        exit(EXIT_FAILURE);
                    }
                }
                //已经处理完所有的连：accept返回-1，errno为EAGAIN
                //出错：返回-1，errno另有其值
                if (conn_sock == -1)
                {
                    if (errno != EAGAIN && errno != ECONNABORTED
                        && errno != EPROTO && errno != EINTR)
                        perror("accept");
                }
                continue;//直接开始下一次循环，也就是不执行这次循环后面的部分了
            }
            if (events[i].events & EPOLLIN) //可读事件
            {
                n = 0;
                while ((nread = read(fd, buf + n, BUFSIZ-1)) > 0)
                {
                    n += nread;
                }
                if (nread == -1 && errno != EAGAIN)
                {
                    perror("read error");
                }
                ev.data.fd = fd;
                ev.events = events[i].events | EPOLLOUT;
                //修改该fd监听事件类型，监测是否可写
                if (epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev) == -1)
                {
                    perror("epoll_ctl: mod");
                }
            }
            if (events[i].events & EPOLLOUT) //可写事件
            {
                sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\nHello World", 11);
                size_t nwrite, data_size = strlen(buf);
                n = data_size;
                while (n > 0)
                {
                    nwrite = write(fd, buf + data_size - n, n);
                    if (nwrite < n)
                    {
                        if (nwrite == -1 && errno != EAGAIN)
                        {
                            perror("write error");
                        }
                        break;
                    }
                    n -= nwrite;
                }
                //写完就关闭该连接socket
                close(fd);
            }
        }
    }

    return 0;
}