from django.db import models

# Create your models here.
class State(models.Model):
    state_id = models.AutoField(primary_key=True)
    name = models.CharField(max_length=256)

class Color(models.Model):
    color_id = models.AutoField(primary_key=True)
    name = models.CharField(max_length=256)

class Team(models.Model):
    team_id = models.AutoField(primary_key=True)
    name = models.CharField(max_length=256)
    state_id = models.ForeignKey(State, on_delete=models.CASCADE)
    color_id = models.ForeignKey(Color, on_delete=models.CASCADE)
    wins = models.IntegerField()
    losses = models.IntegerField()

class Player(models.Model):
    player_id = models.AutoField(primary_key=True)
    team_id = models.ForeignKey(Team, on_delete=models.CASCADE) 
    uniform_num = models.IntegerField()
    first_name = models.CharField(max_length=256)
    last_name = models.CharField(max_length=256)
    mpg = models.IntegerField()
    ppg = models.IntegerField()
    rpg = models.IntegerField()
    apg = models.IntegerField()
    spg = models.FloatField()
    bpg = models.FloatField()
    
    def __str__(self):
        return str(self.player_id) + " " + str(self.team_id.team_id) + " " + str(self.uniform_num) + " " + self.first_name + " " + self.last_name + " " + str(self.mpg) + " " + str(self.ppg) + " " + str(self.rpg) + " " + str(self.apg) + " " + str(self.spg) + " " + str(self.bpg)
