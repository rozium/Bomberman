#include "stdafx.h"
#include "Point.h"

Point::Point() {
	x = 0;
	y = 0;
}

Point::Point(int i, int j) {
	x = i;
	y = j;
}

//getter
int Point::GetAbsis() {
	return x;
}

int Point::GetOrdinat() {
	return y;
}

//setter
void Point::SetAbsis(int Abs) {
	x = Abs;
}

void Point::SetOrdinat(int Ord) {
	y = Ord;
}

int Point::Distance(Point P2){
	return (int)sqrt(pow(GetAbsis() - P2.GetAbsis(), 2) + pow(GetOrdinat() - P2.GetOrdinat(), 2));
}

void Point::Print(){
	std::cout << "(" << GetAbsis() << ", " << GetOrdinat() << ")" << std::endl;
}

bool Point::isSame(Point P1) {
	return (P1.GetAbsis() == GetAbsis() && P1.GetOrdinat() == GetOrdinat());
}