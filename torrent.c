//
// Created by Tory on 4/27/26.
//
#include "torrent.h"

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <pcap/socket.h>
#include <unistd.h>
#include <errno.h>

#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define GETSOCKETERRNO() (errno)
#define PORT 19876
#define GET "GET %s.torrent\n"
#define USAGE "usage: P2PDownloader <tracker_ip> <tracker_port> <filename>\n"
#define GETADDRINFO_FAIL "getaddrinfo() failed.\n"
#define SOCKET_FAIL "socket() failed. (%d)\n"
#define SEND_FAIL "send() failed. (%d)\n"
#define RECVFROM_FAIL "recvfrom() failed. (%d)\n"
#define REMOTE_ADDRESS "Remote address is %s:%s\n"

torrent_info get_torrent_metadata(const char* ip, const char* port, const char* filename) {

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

    // Print remote address
    char address_buffer[100];
    char service_buffer[100];
    getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen,
                address_buffer, sizeof(address_buffer),
                service_buffer, sizeof(service_buffer),
                NI_NUMERICHOST | NI_NUMERICSERV);
    printf(REMOTE_ADDRESS, address_buffer, service_buffer);

    printf("Creating socket...\n");
    SOCKET torrent_server = socket(peer_address->ai_family, peer_address->ai_socktype, peer_address->ai_protocol);
    if (!ISVALIDSOCKET(torrent_server)) {
        fprintf(stderr, SOCKET_FAIL, GETSOCKETERRNO());
        exit(1);
    }

    //Create timeout
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    setsockopt(torrent_server, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));


    //Receive data
    char read[1024];
    struct sockaddr_storage sender_addr;
    socklen_t sender_len = sizeof(sender_addr);
    int attempts = 5;
    ssize_t bytes_received = -1;

    //Format request
    char request[256];
    snprintf(request, sizeof(request), GET, filename);
    printf("%s", request);

    for (int i = 0; i < attempts; i++) {
        printf("Attempt %d...\n", i+1);
        printf("Requesting %s from torrent server...\n", filename);
        if (sendto(torrent_server,
                   request,
                   strlen(request),
                   0,
                   peer_address->ai_addr,
                   peer_address->ai_addrlen) == -1) {
            fprintf(stderr, SEND_FAIL, GETSOCKETERRNO());
            continue;
        }

        bytes_received = recvfrom(torrent_server, read, sizeof(read), 0, (struct sockaddr*)&sender_addr, &sender_len);
        if (bytes_received >= 0) { break; /* message received */ }
        printf("No response, retrying...\n");
    }

    if (bytes_received < 0) {
        fprintf(stderr, RECVFROM_FAIL, GETSOCKETERRNO());
        exit(1);
    }
    printf("Received (%lu bytes): %.*s\n",
           bytes_received, bytes_received, read);

    freeaddrinfo(peer_address);

    printf("Closing connection to torrent server...\n");
    CLOSESOCKET(torrent_server);
}