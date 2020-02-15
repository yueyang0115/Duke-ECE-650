#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "function.h"

using namespace std;

int main(int argc, char * argv[]) {
  int socket_fd;
  const char * hostname = NULL;
  const char * port = "4444";

  socket_fd = build_server(hostname, port);
  int num_players = 3;
  for (int i = 0; i < num_players; i++) {
    int client_connection_fd = server_accept(socket_fd);

    send(client_connection_fd, &i, sizeof(i), 0);
    send(client_connection_fd, &num_players, sizeof(num_players), 0);

    int port;
    recv(client_connection_fd, &port, sizeof(port), 0);
    std::cout << "Player " << i << " is ready to play, port is " << port << " \n";
  }

  close(socket_fd);

  return 0;
}
