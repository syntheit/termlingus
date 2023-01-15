
// Client side C/C++ program to demonstrate Socket programming
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

int watch_server(int sock, char buffer[4096]) {
  while (true) {
    read(sock, buffer, 4096);
    std::cout << "Server: " << buffer << "\n";
    if (std::string(buffer) == "exit")
      break;
    for(int i = 0; i < 4096; ++i)
        buffer[i] = 0;
  }
  return 0;
}

int main(int argc, char const *argv[]) {
  struct sockaddr_in address;
  int sock = 0, valread;
  struct sockaddr_in serv_addr;
  std::string hello = "Hello";
  char buffer[4096] = {0};
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    std::cout << "Failed to create socket\n";
    return -1;
  }

  memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  // Convert IPv4 and IPv6 addresses from text to binary form
  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    std::cout << "Invalid address\n";
    return -1;
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    std::cout << "Failed to connect to server\n";
    return -1;
  }

  send(sock, hello.c_str(), hello.length(), 0);
  std::string input;

  std::thread t1(watch_server, sock, buffer);

  while (getline(std::cin, input)) {
    send(sock, input.c_str(), input.length(), 0);
    if (input == "exit")
      break;
  }

  std::cout << "Server exited\n";

  return 0;
}