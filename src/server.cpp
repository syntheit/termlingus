// Server side C/C++ program to demonstrate Socket programming
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

#define PORT 2947

int watch_client(int new_socket, char buffer[4096]) {
  while (true) {
    read(new_socket, buffer, 4096);
    std::cout << "Client: " << buffer << "\n";
    if (std::string(buffer) == "exit")
      break;
    for (int i = 0; i < 4096; ++i)
      buffer[i] = 0;
  }
  return 0;
}

int main(int argc, char const *argv[]) {
  int server_fd, new_socket, valread;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[4096] = {0};
  std::string hello = "Hello";

  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Forcefully attaching socket to the port 8080
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // Forcefully attaching socket to the port 8080
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

  send(new_socket, hello.c_str(), hello.length(), 0);
  std::string input;

  std::thread t1(watch_client, new_socket, buffer);

  while (getline(std::cin, input)) {
    send(new_socket, input.c_str(), input.length(), 0);
    if (input == "exit")
      break;
  }

  std::cout << "Client exited\n";

  return 0;
}