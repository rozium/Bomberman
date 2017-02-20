// file : Point.h
#include "stdafx.h"

#ifndef POINT_H
#define POINT_H

class Point {
  public:
	//ctor
	Point();
	Point(int xx, int yy);
	
	//getter
	int GetAbsis();
	int GetOrdinat();
	
	//setter
	void SetAbsis(int Abs);
	void SetOrdinat(int Ord);
	
	int Distance(Point P2);
	void Print();
	bool isSame(Point P1);

  private:
	int x;
	int y;

};
#endif