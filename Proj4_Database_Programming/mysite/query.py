import os
os.environ.setdefault("DJANGO_SETTINGS_MODULE", "mysite.settings")

import django
django.setup()
    
from acc_bball.models import State,Color,Team,Player    

def add_state(name):
    State.objects.get_or_create(name = name)
    return

def add_color(name):
    Color.objects.get_or_create(name = name)
    return

def add_team(name, state_id, color_id, wins, losses):
    Team.objects.get_or_create(name = name, state_id = State.objects.get(state_id = state_id), color_id = Color.objects.get(color_id = color_id), wins = wins, losses = losses)

def add_player(team_id, uniform_num, first_name, last_name, mpg, ppg, rpg, apg, spg, bpg):
    Player.objects.get_or_create(team_id = Team.objects.get(team_id = team_id), uniform_num = uniform_num, first_name = first_name, last_name =last_name, mpg = mpg, ppg = ppg, rpg = rpg, apg = apg, spg = spg, bpg =bpg)
    return

def query1(use_mpg, min_mpg, max_mpg, use_ppg, min_ppg, max_ppg, use_rpg, min_rpg, max_rpg, use_apg, min_apg, max_apg, use_spg, min_spg, max_spg, use_bpg, min_bpg, max_bpg):
    ans = Player.objects.all()
    if use_mpg:
        ans = Player.objects.filter(mpg__gte = min_mpg, mpg__lte = max_mpg)
    if use_ppg:
        ans = Player.objects.filter(ppg__gte = min_ppg, ppg__lte = max_ppg)
    if use_rpg:
        ans = Player.objects.filter(rpg__gte = min_rpg, rpg__lte = max_rpg)
    if use_apg:
        ans = Player.objects.filter(apg__gte = min_apg, apg__lte = max_apg)
    if use_spg:
        ans = Player.objects.filter(spg__gte = min_spg, spg__lte = max_spg)
    if use_bpg:
        ans = Player.objects.filter(bpg__gte = min_bpg, bpg__lte = max_bpg)
    print("PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG")
    for player in ans:
         print(player)
    return

def query2(teamcolor):
    ans = Team.objects.filter(color_id__name = teamcolor)
    print("NAME")
    for team in ans:
        print(team.name)
    return

def query3(teamname):
    ans = Player.objects.filter(team_id__name = teamname).order_by('-ppg')
    print("FIRST_NAME LAST_NAME")
    for player in ans:
        print(player.first_name, player.last_name)
    return

def query4(teamstate, teamcolor):
    ans = Player.objects.filter(team_id__state_id__name = teamstate).filter(team_id__color_id__name = teamcolor)
    print("FIRST_NAME LAST_NAME UNIFORM_NUM")
    for player in ans:
        print(player.first_name, player.last_name, player.uniform_num)
    return

def query5(num_wins):
    ans = Player.objects.filter(team_id__wins__gt = num_wins)
    print("FIRST_NAME LAST_NAME NAME WINS")
    for player in ans:
        print(player.first_name, player.last_name, player.team_id.name, player.team_id.wins)
    return

def main():
   query1(1, 35, 40, 0, 0, 0, 0, 5, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0)
   query1(0, 35, 40, 0, 0, 0, 1, 5, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0)
   query2("Green")
   query2("Orange")
   query3("Duke")
   query3("UNC")
   query4("MA", "Maroon")
   query4("NC","DarkBlue")
   query5(13)
   query5(10)
   #add_player(1, 60, "aaa", "bbb", 20, 20, 10, 10, 5.0, 5.0)
   #add_team("myteam", 10, 3, 20, 0)
   #add_state("N/A")
   #add_color("Pink")
   return

if __name__ == "__main__":
    main()
