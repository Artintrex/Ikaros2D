#include "GameHeader.h"

//Initilize static members
Sprite* Box::BoxSprite[BOX_MAX];
Sprite* Javelin::SpearSprite;

bool bPlayer1, bPlayer2, bPlayer3, bPlayer4;
int fScore1, fScore2, fScore3, fScore4;

std::vector<Player*> vPlayers;