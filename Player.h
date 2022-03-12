#pragma once
#include <string>
#include <list>
#include <iterator>
#include <math.h>
class Player
{
private:
	std::string firstName;
	std::string secondName;
public :int points;

public: Player(std::string Fname, std::string sname, int Points)
	{
		firstName = Fname;
		secondName = sname;
		points = Points;
	}


};

class Team
{
private:
	std:: list<Player> Players;
public:std::string TeamName;
	float Totalpoints;
	int cntPlayers;
	int sumPlayerspoints;

public: Team(std::string tname)
{
	TeamName = tname;
}
public:void AddPlayerToTeam(Player pl)
{
	Players.push_back(pl);
	cntPlayers += 1;
	sumPlayerspoints+= pl.points;
	Totalpoints = roundf(((float)sumPlayerspoints / (float)cntPlayers) * 100) / 100;
}
public:void addpoint() 
{
	cntPlayers = 0;
	sumPlayerspoints=0;
	for (std::list<Player>::iterator it = Players.begin(); it != Players.end(); ++it)
	{
		float fds = it->points+1;
		 
		cntPlayers += 1;
		sumPlayerspoints += fds;
		it->points = fds;
		Totalpoints = roundf(((float)sumPlayerspoints / (float)cntPlayers) * 100) / 100;
	}
}


};


