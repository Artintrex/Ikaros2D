#include "GameHeader.h"

//Initilize static members
Sprite* Box::BoxSprite[BOX_MAX];
Sprite* Javelin::SpearSprite;

bool bPlayer1, bPlayer2, bPlayer3, bPlayer4;
std::vector<Player*> vPlayers;