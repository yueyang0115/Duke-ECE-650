#include <iostream>
#include <pqxx/pqxx>
#include <fstream>
#include <string>
#include "exerciser.h"

using namespace std;
using namespace pqxx;

void dropTable(connection *C){
string sql = "DROP TABLE IF EXISTS PLAYER CASCADE;\
DROP TABLE IF EXISTS TEAM CASCADE;\
DROP TABLE IF EXISTS STATE CASCADE;\
DROP TABLE IF EXISTS COLOR CASCADE;";
 work W(*C);
 W.exec( sql );
 W.commit();
 cout << "Table dropped successfully" << endl;  
}

void createTable(string file_name, connection *C){
  string sql;
  string line;
  ifstream myfile (file_name.c_str());
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
  work W(*C);
  W.exec( sql );
  W.commit();
  cout << "Table created successfully" << endl;
}

void readColor(string file_name, connection *C){
    string sql;
  string line;
  string name;
  int color_id;
  ifstream myfile (file_name.c_str());
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
    cout << "Unable to open file"<<endl;
    return;
  }
  work W(*C);
  W.exec( sql );
  W.commit();
  cout << "Table created successfully" << endl;
}
int main (int argc, char *argv[]) 
{
  //Allocate & initialize a Postgres connection object
  connection *C;
  try{
    //Establish a connection to the database
    C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
    if (C->is_open()) {
      cout << "Opened database successfully: " << C->dbname() << endl;
    } else {
      cout << "Can't open database" << endl;
      return 1;
    }
  } catch (const std::exception &e){
    cerr << e.what() << std::endl;
    return 1;
  }

  //TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
  //      load each table with rows from the provided source txt files
  dropTable(C);
  createTable("tables.sql",C);
  //exercise(C);

  //Close database connection
  C->disconnect();
  return 0;
}


