#include <iostream>
#include "apue.h"
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

void print_family(struct addrinfo *aip){
    std::cout << " family ";
    switch (aip->ai_family) {
        case AF_INET:
            std::cout << "inet";
            break;
        case AF_INET6:
            std::cout << "inet6";
            break;
        case AF_UNIX:
            std::cout << "unix socket";
            break;
        case AF_UNSPEC:
            std::cout << "unspecified";
            break;
        default:
            std::cout << "unknown";
    }
}

void print_type(struct addrinfo *aip){
    std::cout << " type ";
    switch (aip->ai_socktype) {
        case SOCK_STREAM:
            std::cout << "stream";
            break;
        case SOCK_DGRAM:
            std::cout << "datagram";
            break;
        case SOCK_SEQPACKET:
            std::cout << "seqpacket";
            break;
        case SOCK_RAW:
            std::cout << "raw";
            break;
        default:
            std::cout << "unknown " << aip->ai_socktype;
    }
}

void print_protocol(struct addrinfo *aip){
    std::cout << " protocol ";
    switch (aip->ai_protocol) {
        case 0:
            std::cout << "default";
            break;
        case IPPROTO_TCP:
            std::cout << "TCP";
            break;
        case IPPROTO_UDP:
            std::cout << "UDP";
            break;
        case IPPROTO_RAW:
            std::cout << "raw";
            break;
        default:
            std::cout << "unknown " << aip->ai_protocol;
    }
}

void print_flags(struct addrinfo *aip){
    std::cout << "flags " ;
    if (aip->ai_flags == 0){
        std::cout << 0;
    }
    else{
        if (aip->ai_flags & AI_PASSIVE){
            std::cout << " passive";
        }
        if (aip->ai_flags & AI_CANONNAME){
            std::cout << " canon";
        }
        if (aip->ai_flags & AI_NUMERICHOST){
            std::cout << " numhost";
        }
        if (aip->ai_flags & AI_NUMERICSERV){
            std::cout << " numserv";
        }
        if (aip->ai_flags & AI_V4MAPPED){
            std::cout << " v4mapped";
        }
        if (aip->ai_flags & AI_ALL){
            std::cout << " all";
        }
    }
}

int main(int argc, char** argv) {
    struct addrinfo *ailist{nullptr}, *aip{nullptr}, hint{};
    struct sockaddr_in *sinp;
    const char *addr;
    int err;
    char abuf[INET_ADDRSTRLEN];
    if (argc != 3){
        err_quit("usage: %s nodename service", argv[0]);
    }
    hint.ai_flags = AI_CANONNAME;
    hint.ai_family = 0;
    hint.ai_socktype = 0;
    hint.ai_protocol = 0;
    hint.ai_addrlen = 0;
    hint.ai_canonname = nullptr;
    hint.ai_addr = nullptr;
    hint.ai_next = nullptr;
    if (err = getaddrinfo(argv[1], argv[2], &hint, &ailist); err != 0){
        err_quit("getaddrinfo error: %s", gai_strerror(err));
    }
    for (aip = ailist; aip != nullptr; aip = aip->ai_next) {
        print_flags(aip);
        print_family(aip);
        print_type(aip);
        print_protocol(aip);
        std::cout << "\n\thost " << (aip->ai_canonname ? aip->ai_canonname : "-");
        if (aip->ai_family == AF_INET){
            sinp = reinterpret_cast<sockaddr_in *>(aip->ai_addr);
            addr = inet_ntop(AF_INET, &sinp->sin_addr, abuf, INET_ADDRSTRLEN);
            std::cout << " address " << (addr ? addr : "unknown");
            std::cout << " port " << ntohs(sinp->sin_port);
        }
        std::cout << '\n';
    }
    return 0;
}
