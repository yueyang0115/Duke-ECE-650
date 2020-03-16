#include "exerciser.h"

void exercise(connection *C)
{
  query1(C, 1, 35, 40, 0, 0, 0, 0, 5, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  query1(C, 0, 35, 40, 0, 0, 0, 1, 5, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  query1(C, 0, 35, 40, 0, 0, 0, 0, 5, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  query2(C, "Orange");
  query3(C, "FloridaState");
  query3(C, "Duke");
  query4(C, "NC", "Red");
  query4(C, "NC", "DarkBlue");
  query5(C, 13);
  add_player(C, 1, 1, "yyyy", "xxxx", 1, 1, 1, 1, 1, 1);
  query1(C, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  query2(C, "b");
  query3(C, "haha");
  query4(C, "NC", "Red");
  query5(C, 5);
  add_player(C, 1, 60, "xxxxx", "yyyy", 20, 20, 10, 10, 5.3, 5.3);
  add_team(C, "team_baldwin", 10, 3, 20, 0);
  add_state(C, "N/A");
  add_color(C, "perry-winkle");
  query1(C, 1, 10, 30, 0, 0, 40, 0, 0, 6, 0, 0, 5, 0, 0.0, 10.0, 0, 0.0, 10.0);
  query2(C, "Gold");
  query3(C, "FloridaState");
  query4(C, "FL", "Maroon");
  query5(C, 8);
}
