#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 

#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <string>


int main(int argc, char *argv[])
{
    const char *groupIP = "239.0.112.1";
    const in_port_t port = 6501;

    int res = 0;


    //creating socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    std::cout << "socket() success" << std::endl;

    // setting SO_REUSEADDR
    u_int yes = 1;
    res = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (int*) &yes, sizeof(yes));
    if( res != 0) {
        std::cout << "failed to set SO_REUSEADDR" << std::endl;
        return -1;
    }

    std::cout << "setsockopt(SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT) success" << std::endl;

    // set up destination address

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    res = bind(sock, (struct sockaddr*) &addr, sizeof(addr));
    if (res != 0) 
    {
        std::cout << "failed to bind socket" << std::endl;
        return -1;   
    }

    std::cout << "bind() success" << std::endl;

    // requesting to join mcast group

    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(groupIP);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    //wlp3s0
        
    
    res = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &mreq, sizeof(mreq));
    if (res != 0) 
    {
        std::cout << "failed to set up joining the group" << std::endl;
        return -1;   
    }

    std::cout << "setsockopt(IPPROTO_IP, IP_ADD_MEMBERSHIP) success" << std::endl;

    char ifName[] = "wlp3s0";
    setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, ifName, strlen(ifName));

    size_t buffLen = 1024;
    char *buff = new char[buffLen];

    while(true)
    {
        memset(buff, 0, buffLen);

        struct sockaddr_in srcAddr;
        socklen_t addrSize = sizeof(srcAddr);

        std::cout << "listening..." << std::endl;

        size_t recvBytes = recvfrom(sock, buff, buffLen, 0, (struct sockaddr*) &srcAddr, &addrSize);

        if (recvBytes < 0) 
        {
            std::cout << "recvfrom() failed. Skipping" << std::endl;
        }

        char *strAddr = new char[15];
        inet_ntop(AF_INET, &srcAddr, strAddr, 15);

        std::cout << "message:" << std::endl <<
                    "\tfsrc: " << strAddr << std::endl <<
                    "\tmsg: " << buff << std::endl <<
                    " -------------------------- " << std::endl;
    }

    return 0;
}