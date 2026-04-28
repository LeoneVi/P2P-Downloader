//
// Created by Tory on 4/27/26.
//

#ifndef P2P_DOWNLOADER_TORRENT_H
#define P2P_DOWNLOADER_TORRENT_H

typedef struct {
    int num_blocks;
    int file_size;
} torrent_info;

torrent_info get_torrent_metadata(const char* ip, const char* port, const char* filename);

#endif //P2P_DOWNLOADER_TORRENT_H
