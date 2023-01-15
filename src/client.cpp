#include "client.h"
#include "gui.h"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

#define PORT 2947

int sock = 0;

int watch_server(char buffer[4096]) {
  while (true) {
    read(sock, buffer, 4096);
    std::cout << "Server: " << buffer << "\n";
    handle_server_data(buffer);
    if (std::string(buffer) == "exit")
      break;
    for (int i = 0; i < 4096; ++i)
      buffer[i] = 0;
  }
  return 0;
}

int send_data_to_server(std::string &data) {
  send(sock, data.c_str(), data.length(), 0);
  return 0;
}

int client_init(std::string &server_ip) {
  struct sockaddr_in address;
  int sock = 0, valread;
  struct sockaddr_in serv_addr;
  char buffer[4096] = {0};
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    std::cout << "Failed to create socket\n";
    return -1;
  }

  memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  // Convert IPv4 and IPv6 addresses from text to binary form
  if (inet_pton(AF_INET, server_ip.c_str(), &serv_addr.sin_addr) <= 0) {
    std::cout << "Invalid server IP address\n";
    return -1;
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    std::cout << "Failed to connect to server\n";
    return -1;
  }

  std::string input;

  std::thread t1(watch_server, buffer);

  //   while (getline(std::cin, input)) {
  //     send(sock, input.c_str(), input.length(), 0);
  //     if (input == "exit")
  //       break;
  //   }

  std::cout << "Server exited\n";

  return 0;
}