#include "stdafx.h"
#include "Player.h"

int Player::number_player = 0;

//ctor
Player::Player() {
	bomb_bag = 1;
	number_player++;
}

//getter
Point Player::GetPos() {
	return Pos;
}

int Player::GetBombBag() {
	return bomb_bag;
}

int Player::GetBombRadius() {
	return bomb_radius;
}

int Player::GetNumberPlayer() {
	return number_player;
}

//setter
void Player::SetPos(Point P) {
	Pos.SetAbsis(P.GetAbsis());
	Pos.SetOrdinat(P.GetOrdinat());
}

void Player::SetBombBag(int b){
	bomb_bag = b;
}

void Player::SetBombRadius(int b){
	bomb_radius = b;
}