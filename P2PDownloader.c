//
// Created by Tory Leone on 4/27/26.
//

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>

#define PORT 19876
#define GET "GET %s.torrent\n"
#define USAGE "usage: P2PDownloader <tracker_ip> <tracker_port> <filename>\n"
#define GETADDRINFO_FAIL "getaddrinfo() failed.\n"
int main(int argc, char* argv[]){
    if(argc < 4){
        fprintf(stderr, USAGE);
        exit(1);
    }

    printf("Configuring address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM; // UDP connection
    hints.ai_protocol = 0;
    struct addrinfo *peer_address;
    if (getaddrinfo(argv[1], argv[2], &hints, &peer_address)) {
        fprintf(stderr, GETADDRINFO_FAIL);
        exit(1);
    }

    printf("Creating socket...\n");
    struct sockaddr_in antelope;
    antelope.sin_family = AF_INET;
    // handle IP address
    char* ip = argv[1];
    inet_aton(ip, &antelope.sin_addr);


    int port = atoi(argv[2]);
    char* filename = argv[3];

    int client_fd = socket(AF_INET, SOCK_DGRAM, port);

    // client downloads torrent metadata


    // if you're waiting for a socket to give something back, it won't work
}