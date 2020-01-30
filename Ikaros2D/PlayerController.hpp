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
			player1->transform->position = Vector3(-20, 20, 0);
			gPlayer = player1->AddComponent<Player>();
			vPlayers.push_back(gPlayer);
		}

		if (bPlayer2) {
			player2 = new GameObject("Player2");
			player2->transform->position = Vector3(0, 20, 0);
			gPlayer = player2->AddComponent<Player>();
			gPlayer->PlayerPath = "2P";
			gPlayer->Left = DIK_LEFT;
			gPlayer->Right = DIK_RIGHT;
			gPlayer->Jump = DIK_UP;
			gPlayer->Attack = DIK_RSHIFT;
			gPlayer->Throw = DIK_RCONTROL;
			gPlayer->Activate = DIK_RETURN;
			vPlayers.push_back(gPlayer);
		}

		if (bPlayer3) {
			player3 = new GameObject("Player3");
			player3->transform->position = Vector3(20, 20, 0);
			gPlayer = player3->AddComponent<Player>();
			gPlayer->PlayerPath = "3P";
			gPlayer->Left = DIK_LEFT;
			gPlayer->Right = DIK_RIGHT;
			gPlayer->Jump = DIK_UP;
			gPlayer->Attack = DIK_RSHIFT;
			gPlayer->Throw = DIK_RCONTROL;
			gPlayer->Activate = DIK_RETURN;
			vPlayers.push_back(gPlayer);
		}

		if (bPlayer4) {
			player4 = new GameObject("Player4");
			player4->transform->position = Vector3(40, 20, 0);
			gPlayer = player4->AddComponent<Player>();
			gPlayer->PlayerPath = "4P";
			gPlayer->Left = DIK_LEFT;
			gPlayer->Right = DIK_RIGHT;
			gPlayer->Jump = DIK_UP;
			gPlayer->Attack = DIK_RSHIFT;
			gPlayer->Throw = DIK_RCONTROL;
			gPlayer->Activate = DIK_RETURN;
			vPlayers.push_back(gPlayer);
		}
	}

	void Update() {

	}
};