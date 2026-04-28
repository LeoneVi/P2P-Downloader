//
// Created by Tory on 4/27/26.
//
#include "torrent.h"

#include <stdlib.h>
#include <stdio.h>

#define USAGE "usage: P2PDownloader <tracker_ip> <tracker_port> <filename>\n"

int main(int argc, char* argv[]){
    // handle command line arguments
    if(argc < 4){
        fprintf(stderr, USAGE);
        exit(1);
    }
    char* ip = argv[1];
    char* port = argv[2];
    char* filename = argv[3];

    printf("Getting torrent metadata...\n");
    torrent_info info = get_torrent_metadata(ip, port, filename);

    /*=============== TCP PHASE ===============*/
    printf("Switching to TCP phase...\n");

    return 0;
}
// client downloads torrent metadata
// if you're waiting for a socket to give something back, it won't work
