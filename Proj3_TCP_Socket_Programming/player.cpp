#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "function.h"

using namespace std;

int main(int argc, char * argv[]) {
  const char * master_hostname = argv[1];
  const char * master_port = "4444";

  if (argc < 2) {
    cout << "Syntax: client <hostname>\n" << endl;
    return 1;
  }

  int master_fd = build_client(master_hostname, master_port);

  int player_id;
  recv(master_fd, &player_id, sizeof(player_id), 0);
  int num_players;
  recv(master_fd, &num_players, sizeof(num_players), 0);

  int player_fd = build_server("");
  int player_fd_port = get_port_num(player_fd);
  cout << "player " << player_id << "'s port is " << player_fd_port << endl;
  send(master_fd, &player_fd_port, sizeof(player_fd_port), 0);
  cout << "Connected as player " << player_id << " out of " << num_players
       << " total players\n";

  close(player_fd);
  close(master_fd);
  return 0;
}
