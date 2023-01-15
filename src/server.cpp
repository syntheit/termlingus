#include "server.h"
#include "gui.h"
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

#define PORT 2947

int new_socket = 0;

int watch_client(char buffer[4096]) {
  while (true) {
    read(new_socket, buffer, 4096);
    handle_client_action(buffer);
    std::cout << "Client did something" << std::endl;
    if (std::string(buffer) == std::string("exit"))
      break;
    for (int i = 0; i < 4096; ++i)
      buffer[i] = 0;
  }
  return 0;
}

int send_data_to_client(std::string &data) {
  send(new_socket, data.c_str(), data.length(), 0);
  return 0;
}

int server_init() {
  int server_fd, valread;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[4096] = {0};

  std::cout << "Waiting for a client to join...\n";

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                           (socklen_t *)&addrlen)) < 0) {
    perror("accept");
    exit(EXIT_FAILURE);
  }

  std::string input;

  std::thread t1(watch_client, buffer);

  //   while (getline(std::cin, input)) {
  //   send(new_socket, input.c_str(), input.length(), 0);
  // if (input == "exit")
  //   break;
  //   }

  std::cout << "Client exited\n";

  return 0;
}