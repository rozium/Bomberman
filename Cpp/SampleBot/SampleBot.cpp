#include "stdafx.h"
#include "json/json.h"
#include <random>
#include "Player.h"
using namespace std;

#define WIDTH 41
#define HEIGHT 41
#define PAGAR '#'
#define DINDING '+'
#define JALAN ' '
#define GOAL 'B'
#define POWER_1 '/'
#define POWER_2 '&'
#define PLAYER_NAME "Um"

char gameblock[HEIGHT][WIDTH];
int size;
Player *P[12]; int p = 0;
Point *Ppos[12]; int pp = 0;
Point *Bpos[20]; int bp = 0; int bf[20]; int br[20];
//Point *Dpos[500]; int dp = 0;
int PLAYER_ID;
Point SuperPowerUP;
bool found = false;
int idx = 0, np = 0, command;
int startX, goalX;
int startY, goalY;
int movee[100];
int minDistance = 999;

void FindUp(int *ax, int *ay, int bx, int by, bool *found, int *idx);
void FindRight(int *ax, int *ay, int bx, int by, bool *found, int *idx);
void FindBottom(int *ax, int *ay, int bx, int by, bool *found, int *idx);
void FindLeft(int *ax, int *ay, int bx, int by, bool *found, int *idx);

int getMovementCommand(Player A, Player B);
int playerDistance(Player A, Player B);
void writeMoveFile(string filePath, int command);
bool inExplosionRange(Point A);
void searchPlayer(char X, int *ii, int *jj, bool *ktm);

int _tmain(int argc, _TCHAR* argv[])
{
	string filePath = argv[2];

	cout << "Args: " << argc << std::endl;
	cout << "Player Key: " << argv[1] << std::endl;
	cout << "File Path: " << argv[2] << std::endl;

	// ReadStateFile (Map.txt)
	int i = 0;
	cout << "Reading state file " << filePath + "/" + "map.txt" << std::endl;
	string fileContent;
	string line;
	ifstream myfile(filePath + "/" + "map.txt");
	if (myfile.is_open())
	{
		while (getline(myfile, line)){
			if (line.find("Player Name:") != string::npos) {
				np++;
			}
		}
		myfile.clear();
		myfile.seekg(0, ios::beg);
		//cout << "np = " << np << endl;
		//cout << "Line = " << line << endl;
		getline(myfile, line);
		getline(myfile, line);
		while (line[0] == PAGAR) {
			for (unsigned int j = 0; j < line.length(); j++) {
				gameblock[i][j] = line[j];
				cout << gameblock[i][j] << " ";
				if (gameblock[i][j] == '$') {
					SuperPowerUP.SetAbsis(i);
					SuperPowerUP.SetOrdinat(j);
				}
				//if (gameblock[i][j] == '+') {
				//	Dpos[dp]->SetAbsis(i);
				//	Dpos[dp]->SetOrdinat(j);
				//}
			}
			cout << endl;
			i++;
			getline(myfile, line);
		}
		size = i;
		for (int b = 0; b < np; b++) {
			bool popo = false;
			int ii = 0;
			int jj = 0;
			searchPlayer(char('A' + b), &ii, &jj, &popo);
			i = ii;
			int j = jj;
			//cout << "A+b = " << char('A' + b) << endl;
			//cout << "gameblock i j = " << gameblock[i][j] << endl;
			if (popo) {
				Ppos[pp] = new Point(i, j);
			} else {
				Ppos[pp] = new Point(999, 999);
			}
			pp++;
			//cout << "player " << pp << " di " << i << "," << j << endl;
		}
		while (getline(myfile, line)) {
			if (line[0] == '-') getline(myfile, line);
			//cout << "Line = " << line << endl;
			P[p] = new Player();
			P[p]->SetPos(*Ppos[p]);
			do {
				if (line.find(PLAYER_NAME) != string::npos) {
					PLAYER_ID = p;
				}
				if (line.find("x:") != string::npos) {
					int count = 0;
					for (size_t offset = line.find("},{"); offset != string::npos; offset = line.find("},{", offset + 3)) {
						count++;
					}
					cout << "COUNT = " << count << endl;
					for (i = 0; i <= count; i++) {
						cout << "LINE = " << line << endl;
						int p1 = line.find("x:"); int p2 = line.find(",y");
						string xx = line.substr(p1 + 2, p2 - p1 - 2);
						line.erase(p1, 2);	
						cout << "LINE = " << line << endl;
						p1 = line.find("y:"); p2 = line.find(",f");
						string yy = line.substr(p1 + 2, p2 - p1 - 2);
						line.erase(p1, 2);
						cout << "LINE = " << line << endl;
						p1 = line.find("e:"); p2 = line.find(",r");
						string fuse = line.substr(p1 + 2, p2 - p1 - 2);
						line.erase(p1-3, 5);
						cout << "LINE = " << line << endl;
						p1 = line.find("us:"); p2 = line.find("}");
						string radius = line.substr(p1 + 3, p2 - p1 - 3);
						line.erase(p1-4, 7);
						cout << "LINE = " << line << endl;

						Bpos[bp] = new Point(atoi(yy.c_str()) - 1, atoi(xx.c_str()) - 1);
						bf[bp] = atoi(fuse.c_str());
						br[bp] = atoi(radius.c_str());
						cout << "bomb " << bp + 1 << ". x,y = " << yy << "," << xx  << ". bf = " << bf[bp] << ".br = " << br[bp] << endl;
						bp++;
					}
				}
				if (line.find("BombBag") != string::npos) {
					string bb = "";
					bb += line[9];
					P[p]->SetBombBag(atoi(bb.c_str()));
				}
				if (line.find("BlastRadius") != string::npos) {
					string radius = "";
					for (unsigned int um = 13; um < line.length(); um++) radius += line[um];
					int iradius = atoi(radius.c_str());
					P[p]->SetBombRadius(iradius);
				}
				getline(myfile, line);
			} while (line[0] != '-');
			p++;
		}
		myfile.close();
	} 

	for (int l = 0; l <100; l++) {
		movee[l] = 7; // do nothing
	}
	startX = P[PLAYER_ID]->GetPos().GetAbsis();
	startY = P[PLAYER_ID]->GetPos().GetOrdinat();

	int ll = 0;
	for (int l = 0; l < np; l++) {
		cout << "l = " << l << endl;
		if (minDistance > playerDistance(*P[PLAYER_ID], *P[l]) && playerDistance(*P[PLAYER_ID], *P[l]) != 0) {
			goalX = P[l]->GetPos().GetAbsis();
			goalY = P[l]->GetPos().GetOrdinat();
			ll = l;
			minDistance = playerDistance(*P[PLAYER_ID], *P[l]);
		}
	}

	cout << "start = " << startX << "," << startY << ". goal = " << goalX << "," << goalY << endl;


	FindUp(&startX, &startY, goalX, goalY, &found, &idx);
	/*
		if (startX > goalX) {
		idx = 0;
	}
	else if ((!found) && (startY < goalY)) {
		idx = 0;
		FindRight(&startX, &startY, goalX, goalY, &found, &idx);
	}
	else if ((!found) && (startX < goalX)) {
		idx = 0;
		FindBottom(&startX, &startY, goalX, goalY, &found, &idx);
	}
	else if ((!found) && (startY > goalY)) {
		idx = 0;
		FindLeft(&startX, &startY, goalX, goalY, &found, &idx);
	} */
	
	if (!found) {
		idx = 0;
		startX = P[PLAYER_ID]->GetPos().GetAbsis();
		startY = P[PLAYER_ID]->GetPos().GetOrdinat();
		goalX = P[ll]->GetPos().GetAbsis();
		goalY = P[ll]->GetPos().GetOrdinat();
		FindRight(&startX, &startY, goalX, goalY, &found, &idx);
	}

	if (!found) {
		idx = 0;
		startX = P[PLAYER_ID]->GetPos().GetAbsis();
		startY = P[PLAYER_ID]->GetPos().GetOrdinat();
		goalX = P[ll]->GetPos().GetAbsis();
		goalY = P[ll]->GetPos().GetOrdinat();
		FindBottom(&startX, &startY, goalX, goalY, &found, &idx);
	}

	if (!found) {
		idx = 0;
		startX = P[PLAYER_ID]->GetPos().GetAbsis();
		startY = P[PLAYER_ID]->GetPos().GetOrdinat();
		goalX = P[ll]->GetPos().GetAbsis();
		goalY = P[ll]->GetPos().GetOrdinat();
		FindLeft(&startX, &startY, goalX, goalY, &found, &idx);
	}

	if (found) {
		for (i = 0; i<30; i++) {
			cout << movee[i] << " -> ";
		}
		cout << endl;
	} else {
		cout << "tidak ketemu" << '\n';
	}
	

	command = movee[0];
	
	Point neP;
	if (inExplosionRange(P[PLAYER_ID]->GetPos())) {
		cout << "MINGGIR COY" << endl;
		neP.SetAbsis(P[PLAYER_ID]->GetPos().GetAbsis() - 1); neP.SetOrdinat(P[PLAYER_ID]->GetPos().GetOrdinat());
		neP.Print();
		int x = 0;
		for (int j = 0; j < bp; j++) {
			if (Bpos[j]->GetAbsis() == neP.GetAbsis() || Bpos[j]->GetOrdinat() == neP.GetOrdinat()) {
				x = j;
				break;
			}
		}
		if ((gameblock[neP.GetAbsis()][neP.GetOrdinat()] == JALAN || gameblock[neP.GetAbsis()][neP.GetOrdinat()] == POWER_1 || gameblock[neP.GetAbsis()][neP.GetOrdinat()] == POWER_2) && (!inExplosionRange(neP) || Bpos[x]->isSame(P[PLAYER_ID]->GetPos()))) {
			command = 1;
		} else {
			neP.SetAbsis(P[PLAYER_ID]->GetPos().GetAbsis()); neP.SetOrdinat(P[PLAYER_ID]->GetPos().GetOrdinat() - 1);
			neP.Print();
			if ((gameblock[neP.GetAbsis()][neP.GetOrdinat()] == JALAN || gameblock[neP.GetAbsis()][neP.GetOrdinat()] == POWER_1 || gameblock[neP.GetAbsis()][neP.GetOrdinat()] == POWER_2) && (!inExplosionRange(neP) || Bpos[x]->isSame(P[PLAYER_ID]->GetPos()))) {
				command = 2;
			}
			else {
				neP.SetAbsis(P[PLAYER_ID]->GetPos().GetAbsis()); neP.SetOrdinat(P[PLAYER_ID]->GetPos().GetOrdinat() + 1);
				neP.Print();
				if ((gameblock[neP.GetAbsis()][neP.GetOrdinat()] == JALAN || gameblock[neP.GetAbsis()][neP.GetOrdinat()] == POWER_1 || gameblock[neP.GetAbsis()][neP.GetOrdinat()] == POWER_2) && (!inExplosionRange(neP) || Bpos[x]->isSame(P[PLAYER_ID]->GetPos()))) {
					command = 3;
				}
				else {
					neP.SetAbsis(P[PLAYER_ID]->GetPos().GetAbsis() + 1); neP.SetOrdinat(P[PLAYER_ID]->GetPos().GetOrdinat());
					neP.Print();
					if ((gameblock[neP.GetAbsis()][neP.GetOrdinat()] == JALAN || gameblock[neP.GetAbsis()][neP.GetOrdinat()] == POWER_1 || gameblock[neP.GetAbsis()][neP.GetOrdinat()] == POWER_2) && (!inExplosionRange(neP) || Bpos[x]->isSame(P[PLAYER_ID]->GetPos()))) {
						command = 4;
					}
					else {
						cout << "Yaudah mati aja.." << endl;
					}
				}
			}
		}
	}

	cout << " 1 command = " << command << endl;
	Point nP;
	if (command == 1) {
		nP.SetAbsis(P[PLAYER_ID]->GetPos().GetAbsis() - 1); nP.SetOrdinat(P[PLAYER_ID]->GetPos().GetOrdinat());
	} else if (command == 2) {
		nP.SetAbsis(P[PLAYER_ID]->GetPos().GetAbsis()); nP.SetOrdinat(P[PLAYER_ID]->GetPos().GetOrdinat()-1);
	} else if (command == 3) {
		nP.SetAbsis(P[PLAYER_ID]->GetPos().GetAbsis()); nP.SetOrdinat(P[PLAYER_ID]->GetPos().GetOrdinat()+1);
	} else if (command == 4) {
		nP.SetAbsis(P[PLAYER_ID]->GetPos().GetAbsis() + 1); nP.SetOrdinat(P[PLAYER_ID]->GetPos().GetOrdinat());
	}
	int commandt = command;
	if (inExplosionRange(nP)) {
		bool Sama = false;
		for (i = 0; i < bp; i++) {
			if (Bpos[i]->isSame(P[PLAYER_ID]->GetPos())) {
				Sama = true;
				cout << " SAMA COY POSISINYA " << endl;
				break;
			} else {
				Sama = false;
			}
		}
		if (!Sama) command = 7;

		Point neeP;
		cout << " 2 COMMAND = " << command << endl;
		cout << " CEK LAGI COY " << endl;
		int commandx = 0, countjalan = 0;
		bool rp = false;
		for (int i = 1; i <= 4; i++) {
			commandx = i;
			if (commandx == 1) {
				neeP.SetAbsis(nP.GetAbsis() - 1); neeP.SetOrdinat(nP.GetOrdinat());
			}
			else if (commandx == 2) {
				neeP.SetAbsis(nP.GetAbsis()); neeP.SetOrdinat(nP.GetOrdinat() - 1);
			}
			else if (commandx == 3) {
				neeP.SetAbsis(nP.GetAbsis()); neeP.SetOrdinat(nP.GetOrdinat() + 1);
			}
			else if (commandx == 4) {
				neeP.SetAbsis(nP.GetAbsis() + 1); neeP.SetOrdinat(nP.GetOrdinat());
			}
			int x = 0;
			for (int j = 0; j < bp; j++) {
				if (Bpos[j]->GetAbsis() == nP.GetAbsis() || Bpos[j]->GetOrdinat() == nP.GetOrdinat()) {
					x = j;
					break;
				}
			}
			if ((gameblock[neeP.GetAbsis()][neeP.GetOrdinat()] == JALAN)) {
				cout << "ADA JALAN !"<< endl;
				countjalan++;
			}
			if (!inExplosionRange(neeP) && (gameblock[neeP.GetAbsis()][neeP.GetOrdinat()] != DINDING && gameblock[neeP.GetAbsis()][neeP.GetOrdinat()] != PAGAR) && bf[x] > 2) {
				cout << " INI BISA COY . COMMAND ke " << i << endl;
				cout << " SEBELUMNYA COMMAND ke " << commandt << endl;
				command = commandt;
				rp = true;
				break;
			}
		}
		if (countjalan == 0 && !rp) {
			Point fp;
			if (command == 1) {
				command = 4;
				fp.SetAbsis(P[PLAYER_ID]->GetPos().GetAbsis() + 1); fp.SetOrdinat(P[PLAYER_ID]->GetPos().GetOrdinat());
			}
			else if (command == 2) {
				command = 3;
				fp.SetAbsis(P[PLAYER_ID]->GetPos().GetAbsis()); fp.SetOrdinat(P[PLAYER_ID]->GetPos().GetOrdinat() + 1);
			}
			else if (command == 3) {
				command = 2;
				fp.SetAbsis(P[PLAYER_ID]->GetPos().GetAbsis()); fp.SetOrdinat(P[PLAYER_ID]->GetPos().GetOrdinat() - 1);
			}
			else if (command == 4) {
				command = 1;
				fp.SetAbsis(P[PLAYER_ID]->GetPos().GetAbsis() - 1); fp.SetOrdinat(P[PLAYER_ID]->GetPos().GetOrdinat());
			}
			if (gameblock[fp.GetAbsis()][fp.GetOrdinat()] == JALAN) {
				//True
			} else {
				if (command == 3) {
					command = 4;
				}
				else if (command == 4) {
					command = 3;
				}
				else if (command == 1) {
					command = 2;
				}
				else if (command == 2) {
					command = 1;
				}
			}
		}

	}

	if ((gameblock[nP.GetAbsis()][nP.GetOrdinat()] == DINDING || isalpha(gameblock[nP.GetAbsis()][nP.GetOrdinat()])) && P[PLAYER_ID]->GetBombBag() > 0) {
		command = 5;
	}
	cout << "final command = " << command << endl;
	writeMoveFile(filePath, command);
	return 0;
}

int getMovementCommand(Player A, Player B){
	
	if ((A.GetPos()).GetAbsis() == (B.GetPos()).GetAbsis()) {
		return (A.GetPos()).GetOrdinat() > (B.GetPos()).GetOrdinat() ? 2 : 3;
	}

	if ((A.GetPos()).GetOrdinat() == (B.GetPos()).GetOrdinat()) {
		return (A.GetPos()).GetAbsis() > (B.GetPos()).GetAbsis() ? 1 : 4;
	}

	return 7;
}

void searchPlayer(char X, int *ii, int *jj, bool *ktm){
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (gameblock[i][j] == X || toupper(gameblock[i][j]) == X) {
				*ktm = true;
				*ii = i;
				*jj = j;
			}
		}
	}
}

int playerDistance(Player A, Player B){
	return A.GetPos().Distance(B.GetPos());
}

void writeMoveFile(string filePath, int command)
{
	cout << "Writing move file " << filePath + "/" + "move.txt" << std::endl;
	ofstream outfile(filePath + "/" + "move.txt");

	if (outfile.is_open())
	{
		//random_device rd; 
		//mt19937 rng(rd());
		//uniform_int_distribution<int> uni(1,4);
		outfile << command << endl;
		outfile.close();
	}
}

bool inExplosionRange(Point A){
	bool inrange = false;
		for (int k = 0; k < bp; k++) {
			cout << "BOMB X Y = "; Bpos[k]->Print(); cout << "Node X Y = "; A.Print(); cout << endl;
			if (Bpos[k]->GetAbsis() == A.GetAbsis()) {
				cout << "SAMA SAMA DI X" << endl;
				if (((Bpos[k]->GetOrdinat() - br[k] <= A.GetOrdinat()) && A.GetOrdinat() <= Bpos[k]->GetOrdinat()) ||
					((Bpos[k]->GetOrdinat() + br[k] >= A.GetOrdinat()) && A.GetOrdinat() >= Bpos[k]->GetOrdinat())){
					inrange = true;
					cout << "IN RANGE !" << endl;
				}
			}
			if (Bpos[k]->GetOrdinat() == A.GetOrdinat()) {
				cout << "SAMA SAMA DI Y" << endl;
				if (((Bpos[k]->GetAbsis() - br[k] <= A.GetAbsis()) && A.GetAbsis() <= Bpos[k]->GetAbsis()) ||
					((Bpos[k]->GetAbsis() + br[k] >= A.GetAbsis()) && A.GetAbsis() >= Bpos[k]->GetAbsis())){
					inrange = true;
					cout << "IN RANGE !" << endl;
				}
			}
	}
	return inrange;
}

void FindUp(int *ax, int *ay, int bx, int by, bool *found, int *idx) {
	//cout << "UP " << "ax = " << *ax << ", ay= " << *ay << ", found = " << *found << ", idx = " << *idx << endl;
	if ((gameblock[*ax - 1][*ay] != JALAN) && (gameblock[*ax - 1][*ay] != DINDING) && (gameblock[*ax - 1][*ay] != POWER_1) && (gameblock[*ax - 1][*ay] != POWER_2)) {
		if (((*ax - 1) == bx) && ((*ay) == by)) {
			*found = true;
			movee[*idx] = 1;
		}
		else {
			// *found = false;
		}
	}
	else {
		//geser naik
		*ax -= 1;
		//simpan
		movee[*idx] = 1;
		int temp = *idx;
		*idx += 1;
		//rekurens
		if (*ax > bx) {
			FindUp(ax, ay, bx, by, found, idx);
		}
		if ((!*found) && (*ay < by)) {
			FindRight(ax, ay, bx, by, found, idx);
		}
		if ((!*found) && (*ax < bx)) {
			FindBottom(ax, ay, bx, by, found, idx);
		}
		if ((!*found) && (*ay > by)) {
			FindLeft(ax, ay, bx, by, found, idx);
		}
	}
}

void FindRight(int *ax, int *ay, int bx, int by, bool *found, int *idx) {
	//cout << "RIGHT " << "ax = " << *ax << ", ay= " << *ay << ", found = " << *found << ", idx = " << *idx << endl;
	if ((gameblock[*ax][*ay + 1] != JALAN) && (gameblock[*ax][*ay + 1] != DINDING) && (gameblock[*ax][*ay + 1] != POWER_1) && (gameblock[*ax][*ay + 1] != POWER_2)) {
		if (((*ax) == bx) && ((*ay + 1) == by)) {
			*found = true;
			movee[*idx] = 3;
		}
		else {
			// *found = false;
		}
	}
	else {
		//geser kanan
		*ay += 1;
		//simpan
		movee[*idx] = 3;
		int temp = *idx;
		*idx += 1;

		//rekurens
		if (*ax > bx) {
			FindUp(ax, ay, bx, by, found, idx);
		}
		if ((!*found) && (*ay < by)) {
			FindRight(ax, ay, bx, by, found, idx);
		}
		if ((!*found) && (*ax < bx)) {
			FindBottom(ax, ay, bx, by, found, idx);
		}
		if ((!*found) && (*ay > by)) {
			FindLeft(ax, ay, bx, by, found, idx);
		}
	}
}

void FindBottom(int *ax, int *ay, int bx, int by, bool *found, int *idx) {
	//cout << "BOTTOM " << "ax = " << *ax << ", ay= " << *ay << ", found = " << *found << ", idx = " << *idx << endl;
	if ((gameblock[*ax + 1][*ay] != JALAN) && (gameblock[*ax + 1][*ay] != DINDING) && (gameblock[*ax + 1][*ay] != POWER_1) && (gameblock[*ax + 1][*ay] != POWER_2)) {
		if (((*ax + 1) == bx) && (*ay == by)) {
			*found = true;
			movee[*idx] = 4;
		}
		else {
			// *found = false;
		}
	}
	else {
		//geser turun
		*ax += 1;
		//simpan
		movee[*idx] = 4;
		int temp = *idx;
		*idx += 1;

		//rekurens
		if (*ax > bx) {
			FindUp(ax, ay, bx, by, found, idx);
		}
		if ((!*found) && (*ay < by)) {
			FindRight(ax, ay, bx, by, found, idx);
		}
		if ((!*found) && (*ax < bx)) {
			FindBottom(ax, ay, bx, by, found, idx);
		}
		if ((!*found) && (*ay > by)) {
			FindLeft(ax, ay, bx, by, found, idx);
		}
	}
}

void FindLeft(int *ax, int *ay, int bx, int by, bool *found, int *idx) {
	//cout << "LEFT " << "ax = " << *ax << ", ay= " << *ay << ", found = " << *found << ", idx = " << *idx << endl;
	if ((gameblock[*ax][*ay - 1] != JALAN) && (gameblock[*ax][*ay - 1] != DINDING) && (gameblock[*ax][*ay - 1] != POWER_1) && (gameblock[*ax][*ay - 1] != POWER_2)) {
		if ((*ax == bx) && ((*ay - 1) == by)) {
			*found = true;
			movee[*idx] = 2;
		}
		else {
			// *found = false;
		}
	}
	else {
		//geser kiri
		*ay -= 1;
		//simpan
		movee[*idx] = 2;
		int temp = *idx;
		*idx += 1;

		//rekurens
		if (*ax > bx) {
			FindUp(ax, ay, bx, by, found, idx);
		}
		if ((!*found) && (*ay < by)) {
			FindRight(ax, ay, bx, by, found, idx);
		}
		if ((!*found) && (*ax < bx)) {
			FindBottom(ax, ay, bx, by, found, idx);
		}
		if ((!*found) && (*ay > by)) {
			FindLeft(ax, ay, bx, by, found, idx);
		}
	}
}