// file : Player.h

#ifndef PLAYER_H
#define PLAYER_H

#include "stdafx.h"
#include "Point.h"

class Player {
  // Method Public
  public:
	//ctor
	Player();
	
	//getter
	Point GetPos();
	int GetBombBag();
	int GetBombRadius();
	static int GetNumberPlayer();
	
	//setter;
	void SetPos(Point P);
	void SetBombBag (int b);
	void SetBombRadius(int b);
	
	static int number_player;

  private:
	Point Pos;
	int bomb_bag;
	int bomb_radius;
};

#endif