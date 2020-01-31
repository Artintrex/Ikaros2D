#pragma once
#include "IkarosCore.h"

extern bool bPlayer1, bPlayer2, bPlayer3, bPlayer4;
extern std::vector<Player*> vPlayers;

class PlayerController : public MonoBehavior {
public:
	PlayerController() {
		type = typeid(*this).name();
		mb_init();
	}

	GameObject* player1;
	GameObject* player2;
	GameObject* player3;
	GameObject* player4;

	void Awake() {
		//FOR TESTING ONLY these need to be set in selection menu
		bPlayer1 = true;
		bPlayer2 = true;
		bPlayer3 = false;
		bPlayer4 = false;
	}

	void Start() {
		vPlayers.clear();
		Player* gPlayer;

		if (bPlayer1) {
			player1 = new GameObject("Player1");
			player1->tag = "Player";
			player1->transform->position = Vector3(-20, 20, 0);
			gPlayer = player1->AddComponent<Player>();
			vPlayers.push_back(gPlayer);
		}

		if (bPlayer2) {
			player2 = new GameObject("Player2");
			player2->tag = "Player";
			player2->transform->position = Vector3(0, 20, 0);
			gPlayer = player2->AddComponent<Player>();
			gPlayer->PlayerPath = "2P";
			gPlayer->iLeft = DIK_LEFT;
			gPlayer->iRight = DIK_RIGHT;
			gPlayer->iJump = DIK_UP;
			gPlayer->iAttack = DIK_RSHIFT;
			gPlayer->iThrow = DIK_RCONTROL;
			gPlayer->iActivate = DIK_RETURN;
			vPlayers.push_back(gPlayer);
		}

		if (bPlayer3) {
			player3 = new GameObject("Player3");
			player3->tag = "Player";
			player3->transform->position = Vector3(20, 20, 0);
			gPlayer = player3->AddComponent<Player>();
			gPlayer->PlayerPath = "3P";
			gPlayer->iLeft = DIK_LEFT;
			gPlayer->iRight = DIK_RIGHT;
			gPlayer->iJump = DIK_UP;
			gPlayer->iAttack = DIK_RSHIFT;
			gPlayer->iThrow = DIK_RCONTROL;
			gPlayer->iActivate = DIK_RETURN;
			vPlayers.push_back(gPlayer);
		}

		if (bPlayer4) {
			player4 = new GameObject("Player4");
			player4->tag = "Player";
			player4->transform->position = Vector3(40, 20, 0);
			gPlayer = player4->AddComponent<Player>();
			gPlayer->PlayerPath = "4P";
			gPlayer->iLeft = DIK_LEFT;
			gPlayer->iRight = DIK_RIGHT;
			gPlayer->iJump = DIK_UP;
			gPlayer->iAttack = DIK_RSHIFT;
			gPlayer->iThrow = DIK_RCONTROL;
			gPlayer->iActivate = DIK_RETURN;
			vPlayers.push_back(gPlayer);
		}
	}

	void Update() {

	}
};