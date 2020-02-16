#include <netdb.h>
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
  if (argc != 4) {
    cout << "ringmaster <port_num> <num_players> <num_hops>" << endl;
    return 1;
  }
  const char * master_port = argv[1];
  int num_players = atoi(argv[2]);
  int num_hops = atoi(argv[3]);
  vector<int> all_player_fd;
  vector<int> all_player_port;
  vector<string> all_player_ip;
  cout << "Potato Ringmaster" << endl;
  cout << "Players = " << num_players << endl;
  cout << "Hops = " << num_hops << endl;

  int socket_fd = build_server(master_port);

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
    cout << "Player " << i << " is ready to play " << endl;
  }

  //master send player its neighbor's ip & port
  for (int i = 0; i < num_players; i++) {
    int neighbor_id = (i + 1) % num_players;
    int neighbor_port = all_player_port[neighbor_id];
    char neighbor_ip[100];
    memset(neighbor_ip, 0, sizeof(neighbor_ip));
    strcpy(neighbor_ip, all_player_ip[neighbor_id].c_str());
    send(all_player_fd[i], &neighbor_port, sizeof(neighbor_port), 0);
    send(all_player_fd[i], &neighbor_ip, sizeof(neighbor_ip), 0);
  }

  //begin playing
  Potato potato;
  potato.num_hops = num_hops;
  if (num_hops == 0) {
    //send potato with num_hops 0 to all players to shut down
    for (int i = 0; i < num_players; i++) {
      send(all_player_fd[i], &potato, sizeof(potato), 0);
    }
  }
  else {
    //send potato to first player
    srand((unsigned int)time(NULL) + num_players);
    int random = rand() % num_players;
    send(all_player_fd[random], &potato, sizeof(potato), 0);
    cout << "Ready to start the game, sending potato to player " << random << endl;

    //receive last potato
    fd_set readfds;
    int nfds = *max_element(all_player_fd.begin(), all_player_fd.end());
    FD_ZERO(&readfds);

    for (int i = 0; i < num_players; i++) {
      FD_SET(all_player_fd[i], &readfds);
    }
    int status = select(nfds + 1, &readfds, NULL, NULL, NULL);
    if (status != 1) {
      cerr << "Error: select error or timed out" << endl;
    }
    for (int i = 0; i < num_players; i++) {
      if (FD_ISSET(all_player_fd[i], &readfds)) {
        recv(all_player_fd[i], &potato, sizeof(potato), MSG_WAITALL);
        break;
      }
    }

    //send potato with num_hops 0 to all players to shut down
    for (int i = 0; i < num_players; i++) {
      send(all_player_fd[i], &potato, sizeof(potato), 0);
    }

    //print potato trace
    cout << "Trace of potato:" << endl;
    for (int i = 0; i < potato.count; i++) {
      cout << potato.path[i];
      if (i != potato.count - 1) {
        cout << ",";
      }
      else {
        cout << endl;
      }
    }
  }
  sleep(1);

  for (int i = 0; i < num_players; i++) {
    close(all_player_fd[i]);
  }
  close(socket_fd);

  return 0;
}
