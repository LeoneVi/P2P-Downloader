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
#include <pcap/socket.h>

#define ISVALIDSOCKET(s) ((s) >= 0)
#define PORT 19876
#define GET "GET %s.torrent\n"
#define USAGE "usage: P2PDownloader <tracker_ip> <tracker_port> <filename>\n"
#define GETADDRINFO_FAIL "getaddrinfo() failed.\n"
#define SOCKET_FAIL "socket() failed.\n"
#define CONNECT_FAIL "connect() failed.\n"
int main(int argc, char* argv[]){
    // handle command line arguments
    if(argc < 4){
        fprintf(stderr, USAGE);
        exit(1);
    }
    char* ip = argv[1];
    char* port = argv[2];
    char* filename = argv[3];

    printf("Configuring address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // accept any addr family
    hints.ai_socktype = SOCK_DGRAM; // UDP connection
    hints.ai_protocol = 0;
    struct addrinfo *peer_address;
    if (getaddrinfo(ip, port, &hints, &peer_address)) {
        fprintf(stderr, GETADDRINFO_FAIL);
        exit(1);
    }

    char address_buffer[100];
    char service_buffer[100];
    getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen,
                address_buffer, sizeof(address_buffer),
                service_buffer, sizeof(service_buffer),
                NI_NUMERICHOST);
    printf("Remote address is %s\n", address_buffer);

    printf("Creating socket...\n");
    SOCKET socket_peer;
    socket_peer = socket(peer_address->ai_family,
                         peer_address->ai_socktype, peer_address->ai_protocol);
    if (!ISVALIDSOCKET(socket_peer)) {
        fprintf(stderr, SOCKET_FAIL);
        exit(1);
    }

    printf("Connecting...\n");
    if (connect(socket_peer,
                peer_address->ai_addr, peer_address->ai_addrlen)) {
        fprintf(stderr, CONNECT_FAIL);
        exit(1);
    }
    freeaddrinfo(peer_address);
    printf("Connected.\n");

    // client downloads torrent metadata
    // if you're waiting for a socket to give something back, it won't work
}