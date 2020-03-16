#include "query_funcs.h"

#include <iomanip>
#include <iostream>
#include <pqxx/pqxx>
#include <sstream>
#include <string>

void add_player(connection * C,
                int team_id,
                int jersey_num,
                string first_name,
                string last_name,
                int mpg,
                int ppg,
                int rpg,
                int apg,
                double spg,
                double bpg) {
  work W(*C);
  stringstream sql;
  sql << "INSERT INTO PLAYER (TEAM_ID, UNIFORM_NUM, FIRST_NAME, LAST_NAME, MPG, PPG, "
         "RPG, APG, SPG, BPG) VALUES ("
      << team_id << ", " << jersey_num << ", " << W.quote(first_name) << ", "
      << W.quote(last_name) << ", " << mpg << ", " << ppg << ", " << rpg << ", " << apg
      << ", " << spg << ", " << bpg << ");";
  W.exec(sql.str());
  W.commit();
}

void add_team(connection * C,
              string name,
              int state_id,
              int color_id,
              int wins,
              int losses) {
  work W(*C);
  stringstream sql;
  sql << "INSERT INTO TEAM (NAME, STATE_ID, COLOR_ID, WINS, LOSSES) VALUES ("
      << W.quote(name) << ", " << state_id << ", " << color_id << ", " << wins << ", "
      << losses << ");";
  W.exec(sql.str());
  W.commit();
}

void add_state(connection * C, string name) {
  work W(*C);
  string sql = "INSERT INTO STATE (NAME) VALUES (" + W.quote(name) + ");";
  W.exec(sql);
  W.commit();
}

void add_color(connection * C, string name) {
  work W(*C);
  string sql = "INSERT INTO COLOR (NAME) VALUES (" + W.quote(name) + ");";
  W.exec(sql);
  W.commit();
}

void query1(connection * C,
            int use_mpg,
            int min_mpg,
            int max_mpg,
            int use_ppg,
            int min_ppg,
            int max_ppg,
            int use_rpg,
            int min_rpg,
            int max_rpg,
            int use_apg,
            int min_apg,
            int max_apg,
            int use_spg,
            double min_spg,
            double max_spg,
            int use_bpg,
            double min_bpg,
            double max_bpg) {
  stringstream sql;
  sql << "SELECT * FROM PLAYER ";
  int use_flag[6] = {use_mpg, use_ppg, use_rpg, use_apg, use_spg, use_bpg};
  int min_array[4] = {min_mpg, min_ppg, min_rpg, min_apg};
  int max_array[4] = {max_mpg, max_ppg, max_rpg, max_apg};
  double min_array2[2] = {min_spg, min_bpg};
  double max_array2[2] = {max_spg, max_bpg};
  string value_name[6] = {"MPG", "PPG", "RPG", "APG", "SPG", "BPG"};
  int num = 0;
  for (int i = 0; i < 4; i++) {
    if (use_flag[i] != 0) {
      if (num == 0) {
        sql << "WHERE ";
      }
      if (num != 0) {
        sql << " AND ";
      }
      sql << value_name[i] << " >= " << min_array[i] << " AND " << value_name[i]
          << " <= " << max_array[i];
      num++;
    }
  }

  for (int j = 0; j < 2; j++) {
    if (use_flag[4 + j] != 0) {
      if (num == 0) {
        sql << "WHERE ";
      }
      if (num != 0) {
        sql << " AND ";
      }
      sql << value_name[4 + j] << " >= " << min_array2[j] << " AND " << value_name[4 + j]
          << " <= " << max_array2[j];
      num++;
    }
  }
  sql << ";";

  nontransaction N(*C);
  result R(N.exec(sql.str()));
  cout << "PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG"
       << endl;
  for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
    cout << c[0].as<int>() << " " << c[1].as<int>() << " " << c[2].as<int>() << " "
         << c[3].as<string>() << " " << c[4].as<string>() << " " << c[5].as<int>() << " "
         << c[6].as<int>() << " " << c[7].as<int>() << " " << c[8].as<int>() << " "
         << fixed << setprecision(1) << c[9].as<double>() << " " << c[10].as<double>()
         << endl;
  }
}

void query2(connection * C, string team_color) {
  work W(*C);
  stringstream sql;
  sql << "SELECT TEAM.NAME FROM TEAM, COLOR WHERE TEAM.COLOR_ID = COLOR.COLOR_ID AND "
         "COLOR.NAME = "
      << W.quote(team_color) << ";";
  W.commit();
  nontransaction N(*C);
  result R(N.exec(sql.str()));
  cout << "NAME" << endl;
  for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
    cout << c[0].as<string>() << endl;
  }
}

void query3(connection * C, string team_name) {
  work W(*C);
  stringstream sql;
  sql << "SELECT FIRST_NAME, LAST_NAME FROM PLAYER, TEAM WHERE PLAYER.TEAM_ID = "
         "TEAM.TEAM_ID AND TEAM.NAME = "
      << W.quote(team_name) << " ORDER BY PPG DESC;";
  W.commit();
  nontransaction N(*C);
  result R(N.exec(sql.str()));
  cout << "FIRST_NAME LAST_NAME" << endl;
  for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
    cout << c[0].as<string>() << " " << c[1].as<string>() << endl;
  }
}

void query4(connection * C, string team_state, string team_color) {
  work W(*C);
  stringstream sql;
  sql << "SELECT FIRST_NAME, LAST_NAME, UNIFORM_NUM FROM PLAYER, STATE, COLOR, TEAM "
         "WHERE ";
  sql << "PLAYER.TEAM_ID = TEAM.TEAM_ID AND TEAM.COLOR_ID = COLOR.COLOR_ID AND "
         "TEAM.STATE_ID = STATE.STATE_ID AND ";
  sql << "STATE.NAME = " << W.quote(team_state) << " AND "
      << "COLOR.NAME = " << W.quote(team_color) << ";";
  W.commit();
  nontransaction N(*C);
  result R(N.exec(sql.str()));
  cout << "FIRST_NAME LAST_NAME UNIFORM_NUM" << endl;
  for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
    cout << c[0].as<string>() << " " << c[1].as<string>() << " " << c[2].as<int>()
         << endl;
  }
}

void query5(connection * C, int num_wins) {
  work W(*C);
  stringstream sql;
  sql << "SELECT FIRST_NAME, LAST_NAME, NAME, WINS FROM PLAYER, TEAM WHERE ";
  sql << "PLAYER.TEAM_ID = TEAM.TEAM_ID AND WINS >" << num_wins << ";";
  W.commit();
  nontransaction N(*C);
  result R(N.exec(sql.str()));
  cout << "FIRST_NAME LAST_NAME NAME WINS" << endl;
  for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
    cout << c[0].as<string>() << " " << c[1].as<string>() << " " << c[2].as<string>()
         << " " << c[3].as<int>() << endl;
  }
}
