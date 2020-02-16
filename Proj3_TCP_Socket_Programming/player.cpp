#include <assert.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <vector>

#include "function.h"
#include "potato.h"
using namespace std;

int main(int argc, char * argv[]) {
  const char * master_hostname = argv[1];
  const char * master_port = argv[2];
  int player_id;
  int num_players;
  if (argc != 3) {
    cout << "player <machine_name> <port_num>" << endl;
    return 1;
  }

  //player connect to master, receive id & num_players from master
  int master_fd = build_client(master_hostname, master_port);
  recv(master_fd, &player_id, sizeof(player_id), 0);
  recv(master_fd, &num_players, sizeof(num_players), 0);

  //player work as server, create and send its port to master
  int player_fd = build_server("");
  int player_fd_port = get_port_num(player_fd);
  send(master_fd, &player_fd_port, sizeof(player_fd_port), 0);
  cout << "Connected as player " << player_id << " out of " << num_players
       << " total players\n";

  //player receive its neighbor's ip & port from master
  int neighbor_port;
  char neighbor_ip[100];
  recv(master_fd, &neighbor_port, sizeof(neighbor_port), MSG_WAITALL);
  recv(master_fd, &neighbor_ip, sizeof(neighbor_ip), MSG_WAITALL);
  //cout << "Neighbor 's ip is " << neighbor_ip << ", neighbor's port is " << neighbor_port<< endl;

  //player work as client, connect to its neighbor's server
  char format_neighbor_port[9];
  sprintf(format_neighbor_port, "%d", neighbor_port);
  int right_neighbor_fd = build_client(neighbor_ip, format_neighbor_port);

  //player work as server, accept neighbor's connection
  string accepted_neighbor_ip;
  int left_neighbor_fd = server_accept(player_fd, &accepted_neighbor_ip);

  //begin playing potato
  Potato potato;
  vector<int> all_connected_fd{right_neighbor_fd, left_neighbor_fd, master_fd};
  fd_set readfds;
  int nfds = *max_element(all_connected_fd.begin(), all_connected_fd.end());

  //receive potato
  while (1) {
    FD_ZERO(&readfds);
    for (int i = 0; i < 3; i++) {
      FD_SET(all_connected_fd[i], &readfds);
    }
    int status = select(nfds + 1, &readfds, NULL, NULL, NULL);
    if (status != 1) {
      cerr << "Error: select error or timed out" << endl;
    }
    for (int i = 0; i < 3; i++) {
      if (FD_ISSET(all_connected_fd[i], &readfds)) {
        recv(all_connected_fd[i], &potato, sizeof(potato), MSG_WAITALL);
        break;
      }
    }

    //receive num_hops =0 potato from master, shut down
    if (potato.num_hops == 0) {
      break;
    }
    potato.num_hops--;
    potato.path[potato.count] = player_id;
    potato.count++;

    //send potato to master
    if (potato.num_hops == 0) {
      send(master_fd, &potato, sizeof(potato), 0);
      cout << "I'm it" << endl;
      continue;
      //break;
    }

    //send potato to neighbor
    srand((unsigned int)time(NULL) + player_id);
    int random = rand() % 2;
    if (random == 0) {
      send(left_neighbor_fd, &potato, sizeof(potato), 0);
      int left_neighbor_id = (player_id + num_players - 1) % num_players;
      cout << "Sending potato to " << left_neighbor_id << endl;
    }
    else {
      send(right_neighbor_fd, &potato, sizeof(potato), 0);
      int right_neighbor_id = (player_id + 1) % num_players;
      cout << "Sending potato to " << right_neighbor_id << endl;
    }
  }
  sleep(1);
  close(left_neighbor_fd);
  close(right_neighbor_fd);
  //close(player_fd);
  close(master_fd);
  return 0;
}
