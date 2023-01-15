#ifndef TERMLINGUS_SERVER_H
#define TERMLINGUS_SERVER_H

#include <stddef.h>
#include <stdint.h>
#include <string>

extern int new_socket;

std::string load_video(const std::string &url);

int watch_client(char buffer[4096]);
int send_data_to_client(std::string &data);
int server_init();

#endif