#ifndef TERMLINGUS_CLIENT_H
#define TERMLINGUS_CLIENT_H

#include <stddef.h>
#include <stdint.h>
#include <string>

std::string load_video(const std::string &url);

extern int sock;

int watch_server(char buffer[4096]);
int send_data_to_server(std::string &data);
int client_init(std::string &server_ip);

#endif