#include <iostream>
#include <pqxx/pqxx>
#include <fstream>
#include <string>
#include "exerciser.h"

using namespace std;
using namespace pqxx;

void executeSQL(string sql, connection *C){
  work W(*C);
  W.exec(sql);
  W.commit();
}

void dropTable(connection *C, string tableName){
string sql = "DROP TABLE IF EXISTS " + tableName + " CASCADE;";
  executeSQL(sql,C);
  // cout << "Drop table " + tableName + " successfully" << endl;  
}

void createTable(string fileName, connection *C){
  string sql;
  string line;
  ifstream myfile (fileName.c_str());
  if (myfile.is_open()){
      while(getline(myfile,line)){
      	  sql.append(line);
      }
       myfile.close();
  }
  else{
    cout << "Unable to open file"<<endl;
    return;
  }
  executeSQL(sql,C);
  //cout << "ALL Tables created successfully" << endl;
}

void insertColor(string fileName, connection *C){
  string sql, line, name;
  int color_id;
  ifstream myfile (fileName.c_str());
  if (myfile.is_open()){
      while(getline(myfile,line)){
	stringstream ss;
	ss << line;
	ss >> color_id >> name;
	add_color(C, name);
      }
      myfile.close();
  }
  else{
    cout << "Unable to open " + fileName <<endl;
    return;
  }
  executeSQL(sql,C);
  //cout << "Insert color successfully" << endl;
}

void insertState(string fileName, connection *C){
  string sql, line, name;
  int state_id;
  ifstream myfile(fileName.c_str());
  if (myfile.is_open()){
      while(getline(myfile,line)){
	stringstream ss;
	ss << line;
	ss >> state_id >> name;
	add_state(C, name);
      }
      myfile.close();
  }
  else{
    cout << "Unable to open " + fileName <<endl;
    return;
  }
  executeSQL(sql,C);
  //cout << "Insert state successfully" << endl;
}

void insertPlayer(string fileName, connection *C){
  string sql, line, first_name, last_name;
  int player_id, team_id, jersey_num, mpg, ppg, rpg, apg;
  double spg, bpg;
  ifstream myfile (fileName.c_str());
  if (myfile.is_open()){
      while(getline(myfile,line)){
	stringstream ss;
	ss << line;
	ss >> player_id >> team_id >> jersey_num >> first_name >> last_name >> mpg >> ppg >> rpg >> apg >> spg >> bpg;
	add_player(C, team_id, jersey_num, first_name, last_name, mpg, ppg, rpg, apg, spg, bpg);
      }
      myfile.close();
  }
  else{
    cout << "Unable to open " + fileName <<endl;
    return;
  }
  executeSQL(sql,C);
  //cout << "Insert player successfully" << endl;
}

void insertTeam(string fileName, connection *C){
  string sql, line, name;
  int team_id, state_id, color_id, wins, losses;
  ifstream myfile (fileName.c_str());
  if (myfile.is_open()){
      while(getline(myfile,line)){
	stringstream ss;
	ss << line;
	ss >> team_id >> name >> state_id >> color_id >> wins >> losses;
	add_team(C, name, state_id, color_id, wins, losses);
      }
      myfile.close();
  }
  else{
    cout << "Unable to open " + fileName <<endl;
    return;
  }
  executeSQL(sql,C);
  //cout << "Insert team successfully" << endl;
}


int main (int argc, char *argv[]) 
{
  connection *C;
  try{
    C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
    if (C->is_open()) {
      //cout << "Opened database successfully: " << C->dbname() << endl;
    } else {
      cout << "Can't open database" << endl;
      return 1;
    }
  } catch (const std::exception &e){
    cerr << e.what() << std::endl;
    return 1;
  }

  dropTable(C, "PLAYER");
  dropTable(C, "TEAM");
  dropTable(C, "STATE");
  dropTable(C, "COLOR");
  createTable("tables.sql", C);
  insertState("state.txt", C);
  insertColor("color.txt", C);
  insertTeam("team.txt", C);
  insertPlayer("player.txt", C); 
  exercise(C);

  C->disconnect();
  return 0;
}
