
#include <iostream>
 
#include <direct.h>
#include <filesystem>

#include <windows.h>
#include <string>
#include <streambuf> 
#include <fstream>
#include <sstream>
#include <list>
#include "Player.h"


using namespace std;


list<string> ToList(string inpp, string delim)
{
	list<string> oo;
	std::string delimiter = delim;
	size_t pos = 0;
	std::string token;
	while ((pos = inpp.find(delimiter)) != std::string::npos) {
		token = inpp.substr(0, pos);
		oo.push_back(token);
		inpp.erase(0, pos + delimiter.length());
	}
	oo.push_back(inpp);
	 
	return oo;
}
list<string> ToList(string inpp, char delim)
{
	list<string> oo;
	std::string delimiter = " ";
	size_t pos = 0;
	std::string token;
	while ((pos = inpp.find(delimiter)) != std::string::npos) {
		token = inpp.substr(0, pos);
		oo.push_back(token);
		inpp.erase(0, pos + delimiter.length());
	}
	//std::cout << inpp << std::endl;
	return oo;
}

wstring GetCurrentDirectory() {
	TCHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
	wstring loc = std::wstring(buffer).substr(0, pos);
	return loc;
}
wstring Date = GetCurrentDirectory() + L"\\date\\";
wstring Rezultate = GetCurrentDirectory() + L"\\Rezultate\\";
string GetContentFile(wstring path)
{
	std::ifstream t(path);
	std::string str;

	t.seekg(0, std::ios::end);
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());
	return str;
}
std::list<Team> StringToListTeams(list<string> inpp, int total)
{
	std::list<Team> toate;
 
	for (std::list<string>::iterator it = inpp.begin(); it != inpp.end(); ++it) {
 
		string firs = it->c_str();
		if (it == inpp.begin() || firs=="")continue;
		 
		//get data of team
		
		list<string> Nm = ToList(firs," ");
		string first = Nm.front();
		int totalpl = std::stoi(first);
		Team tm = Team(firs.erase(0, first.length()+1));

		//get data of player
		for (int i = 0;i < totalpl;i++) 
		{
			it++;
			string playerdata = it->c_str();
			list<string> pldata = ToList(playerdata," ");

			string point= pldata.back();
			std::list<string>::iterator its = pldata.begin();
		
			string fname= its->c_str();
			its++;
			string lname = its->c_str();

 
			Player pl = Player(fname, lname, std::stoi(point));
			tm.AddPlayerToTeam(pl);
		}
		toate.push_front(tm);
		it++;

	}
	return toate;
}

string ParsestringByLength(string str,int len,string FVal) 
{
	int lent = str.length()+FVal.length();
	string strs = str;
	for (int i = lent;i < len;i++) 
	{
		strs += " ";
	}
	strs += FVal;
	return strs;
}


string Controller(string condition ,list<Team> teams ,int froms,int cntTm)
{
	string rez = "";
	list<Team> teamsLoc = teams;
	if (condition == "0")return "";
	if (froms == 1) 
	{
		for (std::list<Team>::iterator it = teams.begin(); it != teams.end(); ++it)
		{
			rez += it->TeamName+'\n';
		}
	}
	else if (froms == 2) 
	{
		teamsLoc.sort([](const Team & a, const Team & b) { return a.Totalpoints > b.Totalpoints; }); 
		int cnt = 0;
		for (std::list<Team>::iterator it = teamsLoc.begin(); it != teamsLoc.end(); ++it)
		{
			 
			Team &tg = *it;
			cnt++;
			if (cnt >= cntTm/2)break;
			rez += it->TeamName + '\n';
		}
		 
	}
	else if (froms == 3)
	{
		int round = 1;
		
		while (teamsLoc.size()>1)
		{
			string rezloc = "\n--- ROUND NO:"+ std::to_string(round)+'\n';
			string Winners = "";
			 
			list<Team> by2;
			list<Team> WinnersL;
			for (std::list<Team>::iterator it = teamsLoc.begin(); it != teamsLoc.end(); ++it)
			{
				 
				by2.push_front(*it);
				if (by2.size() == 2)
				{
					
					Team kk = by2.front();
					Team kk1 = by2.back();
					rezloc += ParsestringByLength(kk.TeamName, 50, "-")+'\n';
					rezloc += kk1.TeamName+'\n';
					if (kk.Totalpoints >= kk1.Totalpoints)
					{
						
						WinnersL.push_front(kk);
						kk.addpoint();
						double pi = kk.Totalpoints;
						std::stringstream stream;
						stream << std::fixed << std::setprecision(2) << pi;
						std::string s = stream.str();
						Winners += ParsestringByLength(kk.TeamName, 50, s) + '\n'; 
					}
					else 
					{
						
						WinnersL.push_front(kk1);
						kk1.addpoint();
						double pi = kk1.Totalpoints;
						std::stringstream stream;
						stream << std::fixed << std::setprecision(2) << pi;
						std::string s = stream.str();
						Winners += ParsestringByLength(kk1.TeamName, 50, s) + '\n';  
					}
					by2.clear();

				}
				 
			}
			teamsLoc = WinnersL;
		
			rez += rezloc +"\nWINNERS OF ROUND NO:"+std::to_string(round)+"\n"+ Winners+"\n\n";
			round++;
		}


		
	}
	else if (froms == 4)
	{

	}
	else if (froms == 5)
	{

	}
	

	
	return rez;
}
void deleteDirectoryContents(const std::wstring& dir_path)
{
	for (const auto& entry : std::filesystem::directory_iterator(dir_path))
		std::filesystem::remove_all(entry.path());
}
void savefile(wstring test, string body)
{
	wstring locpath = Rezultate + L"r" + test + L".out";
	std::ofstream outfile(locpath);

	outfile << body << std::endl;

	outfile.close();
}
void ReadAndConvertData()
{
	//iteratorul nu citeste nimic, pinacind asa.
	for (int i = 1;i <= 15;i++)
	{
		wstring scond = std::to_wstring(i);
		wstring folder = L"t" + scond;
		wstring PathCondIn = Date + folder + L"\\c.in";
		wstring PathDatIn = Date + folder + L"\\d.in";
		
		string EchipeInitiale = GetContentFile(PathDatIn);
		string ConditiiInitiale = GetContentFile(PathCondIn);
		
		
		list<string> Cond = ToList(ConditiiInitiale," ");
		list<string> All = ToList(EchipeInitiale, "\n");

		int totalEchipe = std::stoi(All.front());
	 
		//convert string to list of teams
		list<Team> teams= StringToListTeams(All,totalEchipe);
		int  prconditiions = 0;
		string result = "";
		for (std::list<string>::iterator it = Cond.begin(); it != Cond.end(); ++it)
		{
			prconditiions++;
			result+= Controller(it->c_str(),teams, prconditiions, totalEchipe);
		}


		savefile(scond,result);


	}
 
}

int main()
{

	deleteDirectoryContents(Rezultate);
	ReadAndConvertData();

}



