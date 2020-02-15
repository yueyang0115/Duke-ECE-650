#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <vector>

#include "function.h"

using namespace std;

int main(int argc, char * argv[]) {
  const char * hostname = NULL;
  const char * port = "4444";
  vector<int> all_player_fd;

  int socket_fd = build_server(hostname, port);
  int num_players = 3;
  for (int i = 0; i < num_players; i++) {
    int player_connect_fd = server_accept(socket_fd);
    all_player_fd.push_back(player_connect_fd);

    send(player_connect_fd, &i, sizeof(i), 0);
    send(player_connect_fd, &num_players, sizeof(num_players), 0);

    int port;
    recv(player_connect_fd, &port, sizeof(port), 0);
    std::cout << "Player " << i << " is ready to play, port is " << port << " \n";
  }

  close(socket_fd);

  return 0;
}
