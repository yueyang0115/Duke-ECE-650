#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <vector>

#include "function.h"

using namespace std;

int main(int argc, char * argv[]) {
  const char * master_port = "4444";
  vector<int> all_player_fd;
  vector<int> all_player_port;
  vector<string> all_player_ip;

  int socket_fd = build_server(master_port);
  int num_players = 3;

  for (int i = 0; i < num_players; i++) {
    int player_port;
    string player_ip;
    //master accept player's connection and get its ip
    int player_connect_fd = server_accept(socket_fd, &player_ip);

    //master send player id & nun_players
    send(player_connect_fd, &i, sizeof(i), 0);
    send(player_connect_fd, &num_players, sizeof(num_players), 0);

    //master receive player's port
    recv(player_connect_fd, &player_port, sizeof(player_port), 0);

    all_player_fd.push_back(player_connect_fd);
    all_player_port.push_back(player_port);
    all_player_ip.push_back(player_ip);

    std::cout << "Player " << i << " is ready to play, ip is " << player_ip
              << ", port is " << player_port << endl;
  }

  //master send player its neighbor's ip & port
  for (int i = 0; i < num_players; i++) {
    int neighbor_id = (i + 1) % num_players;
    int neighbor_port = all_player_port[neighbor_id];
    char neighbor_ip[100];
    strcpy(neighbor_ip, all_player_ip[neighbor_id].c_str());
    send(all_player_fd[i], &neighbor_port, sizeof(neighbor_port), 0);
    send(all_player_fd[i], &neighbor_ip, sizeof(neighbor_ip), 0);
  }

  close(socket_fd);

  return 0;
}
