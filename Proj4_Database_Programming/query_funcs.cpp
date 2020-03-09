#include "query_funcs.h"
#include <iostream>
#include <sstream>
#include <pqxx/pqxx>
#include <string>

void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg)
{
  work W(*C);
  stringstream sql;
  sql << "INSERT INTO PLAYER (TEAM_ID, UNIFORM_NUM, FIRST_NAME, LAST_NAME, MPG, PPG, RPG, APG, SPG, BPG) VALUES (" << team_id << ", " << jersey_num << ", " << W.quote(first_name) << ", " << W.quote(last_name) << ", " << mpg << ", " << ppg << ", " << rpg << ", " << apg << ", " << spg << ", " << bpg <<");";
  W.exec(sql.str());
  W.commit();
}


void add_team(connection *C, string name, int state_id, int color_id, int wins, int losses)
{
  work W(*C); 
  stringstream sql;
  sql << "INSERT INTO TEAM (NAME, STATE_ID, COLOR_ID, WINS, LOSSES) VALUES (" << W.quote(name) << ", " << state_id << ", " << color_id << ", " << wins << ", " << losses << ");";
  W.exec(sql.str());
  W.commit();
}

void add_state(connection *C, string name)
{
  work W(*C); 
  string sql = "INSERT INTO STATE (NAME) VALUES (" + W.quote(name) + ");";
  W.exec(sql);
  W.commit();
}

void add_color(connection *C, string name)
{
  work W(*C); 
  string sql = "INSERT INTO COLOR (NAME) VALUES (" + W.quote(name) + ");";
  W.exec(sql);
  W.commit();
}


void query1(connection *C,
	    int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg
            )
{
  stringstream sql;
  sql << "SELECT * FROM PLAYER ";
  int use_flag[6] = {use_mpg, use_ppg, use_rpg, use_apg, use_spg, use_bpg};
  double min_array[6] = {min_mpg, min_ppg, min_rpg, min_apg, min_spg, min_bpg};
  double max_array[6] = {max_mpg,max_ppg,max_rpg,max_apg,max_spg,max_bpg};
  string value_name[6] = {"MPG","PPG","RPG","APG","SPG","BPG"};
  int num = 0;
  for(int i = 0; i < 6; i++){
    if(use_flag[i]!=0){
      if(num == 0){
	sql << "WHERE ";
      }
      if(num != 0){
	sql << " AND ";
      }
      sql << value_name[i] << " >= " << min_array[i] << " AND " << value_name[i] << " <= " << max_array[i]; 
      num++;
    }
  }
  sql << ";";
  
  nontransaction N(*C);
  result R(N.exec(sql.str()));
  cout << "PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG" << endl;
  for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
    cout << c[0].as<int>() << " " << c[1].as<int>() << " " << c[2].as<int>() << " " <<
      c[3].as<string>() << " " << c[4].as<string>() << " " << c[5].as<int>() << " " <<
      c[6].as<int>() << " " << c[7].as<int>() << " " << c[8].as<int>() << " " <<
      c[9].as<double>() << " " << c[10].as<double>() << endl;
  }
}


void query2(connection *C, string team_color)
{
}


void query3(connection *C, string team_name)
{
}


void query4(connection *C, string team_state, string team_color)
{
}


void query5(connection *C, int num_wins)
{
}
